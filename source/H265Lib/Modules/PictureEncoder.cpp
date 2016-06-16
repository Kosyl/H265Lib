#include "PictureEncoder.h"
#include <Common/Logger.h>
#include <Data/Slice.h>

namespace HEVC
{

  HardcodedPictureEncoder::HardcodedPictureEncoder(ParametersBundle parameter_sets, PictureCodingConfiguration *pic_coding_info) :
    parameters(parameter_sets),
    coding_info(pic_coding_info)
  {
  }

  void HardcodedPictureEncoder::encodePicture(Picture &pic)
  {
    LOG_FUNCTION_INDENT(Logger::Dump);
    LOGLN(Logger::Dump, "width: ", pic.width[Luma], ", height: ", pic.height[Luma]);

    for (auto& slice_info : coding_info->slices_configuration)
    {
      //TODO poprawne indeksowanie
      std::shared_ptr<Slice> slice = std::make_shared<Slice>(
        slice_info.start_ctu_address / parameters.Sps->pic_width_in_ctus,
        slice_info.start_ctu_address % parameters.Sps->pic_height_in_ctus,
        slice_info.end_ctu_address - slice_info.start_ctu_address);

      for (int i = slice_info.start_ctu_address; i < slice_info.end_ctu_address; ++i)
      {
        auto ctu = pic.getCTU(i / parameters.Sps->pic_width_in_ctus, i % parameters.Sps->pic_height_in_ctus);
        ctu->slice = slice;
        slice->assignCtu(ctu);
      }

      for (auto y = 0u; y < pic.height_in_ctus; ++y)
      {
        for (auto x = 0u; x < pic.width_in_ctus; ++x)
        {
          CuContext context;
          context.initFromParameters(parameters);
          context.picture = &pic;

          size_t ctu_size = parameters.Sps->ctu_size;

          auto cu_coding_info = std::find_if(slice_info.cus_configuration.begin(), slice_info.cus_configuration.end(), [&](auto& cu)->bool {return cu.pos.x == x*ctu_size && cu.pos.y == y*ctu_size; });

          processQuadtree(*(pic.getCTU(x, y)->cu_tree), context, *cu_coding_info);
        }
      }
    }
  }

  void HardcodedPictureEncoder::processQuadtree(CUQuadTree &tree, CuContext context, CuCodingConfiguration& mode)
  {
    bool bigger_than_max_transform = tree.size() > context.max_tb_size;
    bool overlaps_picture_boundaries =
      tree.pos.x + tree.size() > context.pic_width
      || tree.pos.y + tree.size() > context.pic_height;

    //bool hasToSplit = bigger_than_max_transform || overlaps_picture_boundaries;
    bool hasToSplit = mode.is_split;

    if (hasToSplit)
    {
      tree.buildSubtrees(context.pic_width, context.pic_height);
      for (auto &subtree : tree.subtrees) //kolejnosc z
      {
        if (subtree != nullptr)
        {
          auto cu_coding_info = std::find_if(mode.cus_configuration.begin(), mode.cus_configuration.end(), [&](auto& cu)->bool {return cu.pos.x == subtree->pos.x && cu.pos.y == subtree->pos.y; });

          processQuadtree(*subtree, context, *cu_coding_info);
        }
      }
    }
    else //lisc
    {
      tree.buildLeaf();

      processCu(*(tree.getCU()), context, mode);
    }
  }

  void HardcodedPictureEncoder::processCu(CU& cu, CuContext context, CuCodingConfiguration& mode)
  {
    LOG_INDENT(Logger::Dump);
    LOGLN(Logger::Dump, "CU(", cu.pos.x, ",", cu.pos.y, ") ", cu.size(), "x", cu.size());
    LOGLN(Logger::Dump, "luma: ", mode.intra_modes_luma[0], ", chroma: ", mode.intra_mode_chroma);

    cu.prediction_type = PredictionType::Intra;
    cu.chroma_prediction_derivation_type = 4;
    cu.cu_qp_delta = 0;
    cu.transquant_bypass_on = false;
    cu.setPartitionMode(mode.partition_mode);

    {
      LOG_INDENT(Logger::Dump);
      for (size_t i = 0; i < cu.prediction_units.size(); ++i)
      {
        auto pu = cu.prediction_units[i];
        pu->setIntraPredictionMode(mode.intra_modes_luma[i], mode.getChromaDerivationType(mode.intra_modes_luma[i]));

        LOGLN(Logger::Dump, "PU(", pu->pos.x, ",", pu->pos.y, ") ", pu->size(), "x", pu->size(),
          ", luma: ", pu->intra_luma_prediction_mode, ", chroma: ", pu->intra_chroma_prediction_mode, 
          "(", pu->intra_chroma_mode_derivation_type, ")");
      }
    }

    cu.transform_tree = std::make_shared<TUQuadTree>(cu.pos.x, cu.pos.y, cu.size());

    {
      LOG_INDENT(Logger::Dump);
      prepareTransformTree(*cu.transform_tree, context, mode.tus_configuration);
    }

    auto tu_iterator = cu.transform_tree->begin();
    do
    {
      TU& tu = **tu_iterator;
      
      processTu(tu);

      ++tu_iterator;
    } while (!tu_iterator.atEnd());
  }

  void HardcodedPictureEncoder::prepareTransformTree(TUQuadTree &tree, CuContext& context, TuCodingConfiguration& mode) const
  {
    bool hasToSplit = mode.is_split;

    if (hasToSplit)
    {
      tree.buildSubtrees(context.pic_width, context.pic_height);
      auto subtree = tree.subtrees.begin(); 
      auto tu_mode = mode.tus_configuration.begin();

      for (; subtree != tree.subtrees.end(); ++subtree, ++tu_mode) //kolejnosc z
      {
        if (*subtree != nullptr)
        {
          prepareTransformTree(**subtree, context, *tu_mode);
        }
      }
    }
    else //lisc
    {
      tree.buildLeaf();
     
      LOGLN(Logger::Dump, "TU(", tree.getTU()->pos.x, ",", tree.getTU()->pos.y, ") ", tree.getTU()->size(), "x", tree.getTU()->size());
    }
  }

  void HardcodedPictureEncoder::processTu(TU& tu)
  {
    LOGLN(Logger::Encoder, "TU(", tu.pos.x, ",", tu.pos.y, ") ", tu.size(), "x", tu.size());
    LOG_INDENT(Logger::Encoder);


  }
}
