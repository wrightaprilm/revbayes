#ifndef CharacterHistoryAugmentedProposal_H
#define CharacterHistoryAugmentedProposal_H

#include <iosfwd>

#include "Proposal.h"

#include "RlMonitor.h"
#include "RevObject.h"
#include "RevPtr.h"
#include "RevVariable.h"
#include "TypedDagNode.h"
#include "AbstractPhyloCTMCSiteHomogeneous.h"

namespace RevBayesCore {
class DagNode;
class StateDependentSpeciationExtinctionProcess;
class AbstractHomologousDiscreteCharacterData;
class Tree;
template <class variableType> class StochasticNode;
    

class CharacterHistoryAugmentedProposal : public Proposal {
        
    public:
        
        CharacterHistoryAugmentedProposal(StochasticNode<AbstractHomologousDiscreteCharacterData> *n);                                                                              //!<  constructor
        void                                            cleanProposal(void);                                                                //!< Clean up proposal
        CharacterHistoryAugmentedProposal*              clone(void) const;                                                                  //!< Clone object
        double                                          doProposal(void);                                                                   //!< Perform proposal
        const std::string&                              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        double                                          getProposalTuningParameter(void) const;

        void                                            prepareProposal(void);                                                              //!< Prepare the proposal
        void                                            printParameterSummary(std::ostream &o, bool name_only) const;                                       //!< Print the parameter summary
        void                                            setProposalTuningParameter(double tp);
        void                                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:

        void                                            swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        AbstractPhyloCTMCSiteHomogeneous<RevBayesCore::DnaState>*      ctmc_dist;

    private:
        
        StochasticNode<AbstractHomologousDiscreteCharacterData>*     ctmc;                                            //!< The variable the Proposal is working on
        TypedDagNode<Tree>*                             tree;

        
    };
    
}

#endif

