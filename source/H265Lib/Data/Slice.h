#pragma once

#include <Common/Enums.h>
#include <memory>

namespace HEVC
{
	class CTU;

	class Slice
	{
		private:

			std::vector<std::weak_ptr<CTU>> ctus;

			size_t begin_ctu_x;
			size_t begin_ctu_y;
			size_t length_in_ctus;

		public:

			~Slice();
			Slice(size_t beginCtuX, size_t beginCtuY, size_t lengthInCtus);
		
			int slice_idx{0};
			SliceType type{ SliceType::I };

			bool sao_enabled[2]{1,1}; //luma, chroma
			bool deblocking_filter_enabled{1};
			bool loop_filter_enabled{1};
			int beta_offset{0};
			int tc_offset{0};

			int slice_qp_delta{0};
			int chroma_qp_offset[3]{0,0,0}; //luma (0, nieuzywany), cb, cr

			bool entropy_coder_sync_enabled{0};

			int getQPDelta( ImgComp comp )
			{
				if (comp == Luma)
					return slice_qp_delta;

				return slice_qp_delta + chroma_qp_offset[comp];
			}

			void assignCtu(std::shared_ptr<CTU> ctu)
			{
				ctus.push_back(std::weak_ptr<CTU>(ctu));
			}
	};
}