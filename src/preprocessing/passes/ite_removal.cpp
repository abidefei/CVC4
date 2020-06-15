/*********************                                                        */
/*! \file ite_removal.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andres Noetzli
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Remove ITEs from the assertions
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "preprocessing/passes/ite_removal.h"

#include "theory/rewriter.h"

namespace CVC4 {
namespace preprocessing {
namespace passes {

using namespace CVC4::theory;

IteRemoval::IteRemoval(PreprocessingPassContext* preprocContext)
    : PreprocessingPass(preprocContext, "ite-removal")
{
}

PreprocessingPassResult IteRemoval::applyInternal(AssertionPipeline* assertions)
{
  d_preprocContext->spendResource(ResourceManager::Resource::PreprocessStep);

  // Remove all of the ITE occurrences and normalize
  for (unsigned i = 0, size = assertions->size(); i < size; ++i)
  {
    std::vector<theory::TrustNode> newAsserts;
    TrustNode trn = d_preprocContext->getIteRemover()->run(
        (*assertions)[i], newAsserts, assertions->getIteSkolemMap(), true);
    // process
    assertions->replace(i, trn.getNode());
    for (unsigned j = 0, nnasserts = newAsserts.size(); j < nnasserts; j++)
    {
      assertions->ref().push_back(newAsserts[j].getNode());
    }
  }
  for (unsigned i = 0, size = assertions->size(); i < size; ++i)
  {
    assertions->replace(i, Rewriter::rewrite((*assertions)[i]));
  }

  return PreprocessingPassResult::NO_CONFLICT;
}


}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC4
