#include "IntraPred.h"

namespace HEVC
{
	IntraPred::IntraPred( std::shared_ptr<SequenceParameterSet> parameters ):
		sps( parameters )
	{
	}

	int IntraPred::getFilteringThreshold( size_t block_size )
	{
		int thresh = 10;
		switch( block_size )
		{
			case 8:
				thresh = 7;
				break;
			case 16:
				thresh = 1;
				break;
			case 32:
				thresh = 0;
				break;
		};

		assert( thresh != 10 ); //dla block_size==4, ale dla 4 nigdy nie powinno wejsc do tej metody

		return thresh;
	}

	bool IntraPred::isFilteringRequired( ImgComp img_comp, int mode_idx, size_t block_size ) const
	{
		if( img_comp != Luma )
			return false;

		if( mode_idx == IntraMode_DC || block_size == 4 )
			return false;

		int dist = std::min( std::abs( mode_idx - 10 ), std::abs( mode_idx - 26 ) );
		if( dist <= getFilteringThreshold( block_size ) )
			return false;

		return true;
	}

	Sample IntraPred::filtreSample( Sample main, Sample left, Sample right )
	{
		return ( left + 2 * main + right + 2 ) >> 2;
	}

	void IntraPred::filtreReferenceSamples( IntraReferenceSamples &samples )
	{
		Sample corner = samples.Corner;
		samples.Corner = filtreSample( corner, samples.Left[ 0 ], samples.Top[ 0 ] );

		Sample prevLeft = corner, prevTop = corner, currLeft, currTop;
		int limit = 2 * samples.block_size - 1;
		for( int x = 0; x < limit; ++x )
		{
			currLeft = samples.Left[ x ];
			currTop = samples.Top[ x ];
			samples.Left[ x ] = filtreSample( prevLeft, samples.Left[ x ], samples.Left[ x + 1 ] );
			samples.Top[ x ] = filtreSample( prevTop, samples.Top[ x ], samples.Top[ x + 1 ] );
			prevLeft = currLeft;
			prevTop = currTop;
		}
	}

	bool IntraPred::isSmoothingRequired( ImgComp img_comp, IntraReferenceSamples &samples ) const
	{
		bool smoothPossible = ( samples.block_size == 32 ) && sps->strong_intra_smoothing_enabled_flag && img_comp == Luma;
		if( !smoothPossible )
			return false;

		Sample limit = 1 << ( sps->bit_depth_luma - 5 );
		auto condLeft = std::abs( samples.Corner + samples.Left[ 2 * samples.block_size - 1 ] - 2 * samples.Left[ samples.block_size - 1 ] );
		auto condTop = std::abs( samples.Corner + samples.Top[ 2 * samples.block_size - 1 ] - 2 * samples.Top[ samples.block_size - 1 ] );

		bool smoothRequired = condLeft < limit && condTop < limit;
		return smoothRequired;
	}

	void IntraPred::smoothReferenceSamples( IntraReferenceSamples &samples )
	{
		assert( samples.block_size == 32 );

		int limit = 2 * samples.block_size - 1;
		for( int x = 0; x < limit; ++x )
		{
			samples.Left[ x ] = ( ( 63 - x ) * samples.Corner + ( x + 1 ) * samples.Left[ limit ] + 32 ) >> 6;
			samples.Top[ x ] = ( ( 63 - x ) * samples.Corner + ( x + 1 ) * samples.Top[ limit ] + 32 ) >> 6;
		}
	}

	IntraMode& IntraPred::getPredictionStrategy( int mode_idx )
	{
		switch( mode_idx )
		{
			case IntraMode_Planar:
				return mode_planar;
			case IntraMode_DC:
				return mode_dc;
			case IntraMode_Horizontal:
			case IntraMode_Vertical:
				return mode_linear;
			default:
				return mode_angular;
		}
	}

	bool IntraPred::checkSampleAvailability( Position curr_luma_position, Position target_luma_position, Picture &pic )
	{
		auto picWidth = pic.width[ Luma ];
		auto picHeight = pic.height[ Luma ];

		bool puOutsidePic =
			( target_luma_position.x < 0 ) ||
			( target_luma_position.y < 0 ) ||
			( target_luma_position.x >= static_cast<int>( picWidth ) ) ||
			( target_luma_position.y >= static_cast<int>( picHeight ) );

		size_t currIdx = pic.getZScanIdx( curr_luma_position.x, curr_luma_position.y, true );
		size_t targetIdx = pic.getZScanIdx( target_luma_position.x, target_luma_position.y, true );
		bool puAlreadyCalc = targetIdx < currIdx;

		//todo: sprawdzanie intra/inter i flagi constrained_intra_pred

		if( puOutsidePic || !puAlreadyCalc )
			return false;

		return true;
	}

	IntraReferenceSamples IntraPred::calcReference( MatrixRef<Sample> source, TB &tb, Picture &pic )
	{
		IntraReferenceAvailability avail( tb.size( ) );
		IntraReferenceSamples samples( tb.size( ) );

		bool atLeasOneAvailable = false;

		Position tbLumaPos = pic.scalePositionToLuma( tb.pos, tb.plane );
		Position targetLumaPos = pic.scalePositionToLuma( tb.pos.move( -1, -1 ), tb.plane );

		//corner
		avail.Corner = checkSampleAvailability( tbLumaPos, targetLumaPos, pic );
		if( avail.Corner )
		{
			samples.Corner = source( tb.pos.x - 1, tb.pos.y - 1 );
			atLeasOneAvailable = true;
		}

		int offsetLimit = 2 * tb.size( );

		for( int offset = 0; offset < offsetLimit; ++offset )
		{
			//left
			targetLumaPos = pic.scalePositionToLuma( tb.pos.move( -1, offset ), tb.plane );
			avail.Left[ offset ] = checkSampleAvailability( tbLumaPos, targetLumaPos, pic );
			if( avail.Left[ offset ] )
			{
				samples.Left[ offset ] = source( tb.pos.x - 1, tb.pos.y + offset );
				atLeasOneAvailable = true;
			}

			//top
			targetLumaPos = pic.scalePositionToLuma( tb.pos.move( offset, -1 ), tb.plane );
			avail.Top[ offset ] = checkSampleAvailability( tbLumaPos, targetLumaPos, pic );
			if( avail.Top[ offset ] )
			{
				samples.Top[ offset ] = source( tb.pos.x + offset, tb.pos.y - 1 );
				atLeasOneAvailable = true;
			}
		}

		if( !atLeasOneAvailable )
		{
			Sample defaultSample = pic.pps->sps->getDefaultSampleValue( tb.plane );
			samples.Corner = defaultSample;
			for( int offset = 0; offset < offsetLimit; ++offset )
			{
				samples.Left[ offset ] = defaultSample;
				samples.Top[ offset ] = defaultSample;
			}
		}
		else
		{
			fillMissingReferences( samples, avail );
		}

		return samples;
	}

	void IntraPred::fillMissingReferences( IntraReferenceSamples &samples, IntraReferenceAvailability &availability )
	{
		size_t refSize = 2 * samples.block_size;

		if( availability.Left[ refSize - 1 ] == false )
		{
			for( int idx = refSize - 2; idx >= 0; --idx )
			{
				//scan left
				if( availability.Left[ idx ] )
				{
					samples.Left[ refSize - 1 ] = samples.Left[ idx ];
					availability.Left[ refSize - 1 ] = true;
					break;
				}
			}

			if( availability.Left[ refSize - 1 ] == false && availability.Corner == true )
			{
				samples.Left[ refSize - 1 ] = samples.Corner;
				availability.Left[ refSize - 1 ] = true;
			}

			if( availability.Left[ refSize - 1 ] == false )
			{
				for( size_t idx = 0; idx < refSize; ++idx )
				{
					//scan top
					if( availability.Top[ idx ] )
					{
						samples.Left[ refSize - 1 ] = samples.Top[ idx ];
						availability.Left[ refSize - 1 ] = true;
						break;
					}
				}
			}
		}

		for( int idx = refSize - 2; idx >= 0; --idx )
		{
			if( !availability.Left[ idx ] )
			{
				samples.Left[ idx ] = samples.Left[ idx + 1 ];
			}
		}
		if( !availability.Corner )
			samples.Corner = samples.Left[ 0 ];
		if( !availability.Top[ 0 ] )
			samples.Top[ 0 ] = samples.Corner;
		for( size_t idx = 1; idx < refSize; ++idx )
		{
			if( !availability.Top[ idx ] )
			{
				samples.Top[ idx ] = samples.Top[ idx - 1 ];
			}
		}
	}

	Matrix<Sample> IntraPred::calcPred( IntraReferenceSamples samples, ImgComp img_comp, int mode_idx )
	{
		auto size = samples.block_size;

		if( isFilteringRequired( img_comp, mode_idx, size ) )
		{
			if( isSmoothingRequired( img_comp, samples ) )
				smoothReferenceSamples( samples );
			else
				filtreReferenceSamples( samples );
		}

		IntraMode &strategy = getPredictionStrategy( mode_idx );

		return strategy.calcPred( samples, img_comp, mode_idx );
	}
}