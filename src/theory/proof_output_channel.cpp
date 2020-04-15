/*********************                                                        */
/*! \file proof_output_channel.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The Evaluator class
 **
 ** The Evaluator class.
 **/

#include "theory/proof_output_channel.h"

namespace CVC4 {
namespace theory {

ProofOutputChannel::ProofOutputChannel(OutputChannel& out,
                                       context::UserContext* u)
    : d_out(out), d_lemPfGen(u)
{
}
void ProofOutputChannel::conflict(Node conf, ProofGenerator* pfg)
{
  Assert(pfg != nullptr);
  Node ckey = getConflictKeyValue(conf);
  d_lemPfGen[ckey] = pfg;
  d_out.conflict(conf);
}

LemmaStatus ProofOutputChannel::lemma(Node lem,
                                      ProofGenerator* pfg,
                                      bool removable,
                                      bool preprocess,
                                      bool sendAtoms)
{
  Assert(pfg != nullptr);
  Node lkey = getLemmaKeyValue(lem);
  d_lemPfGen[lkey] = pfg;
  return d_out.lemma(lem, removable, preprocess, sendAtoms);
}

std::shared_ptr<ProofNode> ProofOutputChannel::getProof(Node n) const
{
  NodeProofGenMap::const_iterator it = d_lemPfGen.find(n);
  Assert(it != d_lemPfGen.end());
  std::shared_ptr<ProofNode> ret = (*it).second->getProof(n);
  Assert(ret != nullptr)
      << "ProofOutputChannel::getProof: could not generate proof for " << n
      << std::endl;
  return ret;
}

Node ProofOutputChannel::getConflictKeyValue(Node conf)
{
  return conf.negate();
}

Node ProofOutputChannel::getLemmaKeyValue(Node lem) { return lem; }

}  // namespace theory
}  // namespace CVC4