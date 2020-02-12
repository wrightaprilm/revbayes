#include "CharacterHistoryAugmentedProposal.h"

#include <stddef.h>
#include <vector>

#include "RbException.h"
//#include "StateDependentSpeciationExtinctionProcess.h"
#include "Tree.h"
#include "AbstractHomologousDiscreteCharacterData.h"
#include "Cloneable.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DagNode.h"


namespace RevBayesCore { class DagNode; }

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class characterType>
CharacterHistoryAugmentedProposal<characterType>::CharacterHistoryAugmentedProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n ) :
        Proposal(), ctmc( n )
{
    std::cout << "I am in the core 1" << std::endl;
    // tell the base class to add the node
    addNode( ctmc );
    
    ctmc_dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<characterType>* >( &ctmc->getDistribution() );
    tree = const_cast<TypedDagNode<Tree>* >( ctmc_dist->getTree() );


//    distribution = dynamic_cast< StateDependentSpeciationExtinctionProcess* >( &ctmc->getDistribution() );
//    if ( distribution == NULL )
//    {
//        throw RbException("The CharacterHistoryAugmentedProposal is currently only implemented for CDBDP distributions.");
//    }
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
template<class characterType>
void CharacterHistoryAugmentedProposal<characterType>::cleanProposal( void )
{
    
    
}


template<class characterType>
CharacterHistoryAugmentedProposal<characterType>* CharacterHistoryAugmentedProposal<characterType>::clone( void ) const
{
    return new CharacterHistoryAugmentedProposal( *this );
}

/**
 * Perform the proposal.
 *
 * \return The hastings ratio.
 */
template<class characterType>
double CharacterHistoryAugmentedProposal<characterType>::doProposal( void )
{

//    size_t num_nodes = ctmc_dist->getValue().getNumberOfNodes();
//    std::vector<std::string> character_histories(num_nodes);
//    ctmc_dist->drawStochasticCharacterMap(character_histories);

    return 0.0;
}


template<class characterType>
const std::string& CharacterHistoryAugmentedProposal<characterType>::getProposalName( void ) const
{
    static std::string name = "CharacterHistoryAugmentedProposal";
    return name;
}

template<class characterType>
double CharacterHistoryAugmentedProposal<characterType>::getProposalTuningParameter( void ) const
{
    // this proposal has no tuning parameter
    return RbConstants::Double::nan;
}

template<class characterType>
void CharacterHistoryAugmentedProposal<characterType>::prepareProposal( void )
{
    
}

template<class characterType>
void CharacterHistoryAugmentedProposal<characterType>::printParameterSummary(std::ostream &o, bool name_only) const
{


}

template<class characterType>
void CharacterHistoryAugmentedProposal<characterType>::undoProposal( void )
{
    
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old ctmc that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
template<class characterType>
void CharacterHistoryAugmentedProposal<characterType>::swapNodeInternal(DagNode *oldN, DagNode *newN)
{

    ctmc = static_cast< StochasticNode<AbstractHomologousDiscreteCharacterData>* >(newN) ;

    ctmc_dist = dynamic_cast< StateDependentSpeciationExtinctionProcess* >( &ctmc->getDistribution() );
    if ( ctmc_dist == NULL )
    {
        throw RbException("The CharacterHistoryAugmentedProposal is currently only implemented for CDBDP distributions.");
    }
}

template<class characterType>
void CharacterHistoryAugmentedProposal<characterType>::setProposalTuningParameter(double tp)
{
    // this proposal has no tuning parameter: nothing to do
}

template<class characterType>
void CharacterHistoryAugmentedProposal<characterType>::tune( double rate )
{
    
    
}

