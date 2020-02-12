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
CharacterHistoryAugmentedProposal::CharacterHistoryAugmentedProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n ) :
        Proposal(), ctmc( n )
{
    std::cout << "I am in the core 1" << std::endl;
    // tell the base class to add the node
    addNode( ctmc );
    
    ctmc_dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<RevBayesCore::DnaState>* >( &ctmc->getDistribution() );
    std::cout << "I am in the core 2" << std::endl;
    tree = const_cast<TypedDagNode<Tree>* >( ctmc_dist->getTree() );
    std::cout << "I am in the core 3" << std::endl;


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
void CharacterHistoryAugmentedProposal::cleanProposal( void )
{
    
    
}


CharacterHistoryAugmentedProposal* CharacterHistoryAugmentedProposal::clone( void ) const
{
    return new CharacterHistoryAugmentedProposal( *this );
}

/**
 * Perform the proposal.
 *
 * \return The hastings ratio.
 */
double CharacterHistoryAugmentedProposal::doProposal( void )
{

//    size_t num_nodes = ctmc_dist->getValue().getNumberOfNodes();
//    std::vector<std::string> character_histories(num_nodes);
//    ctmc_dist->drawStochasticCharacterMap(character_histories);

    return 0.0;
}


const std::string& CharacterHistoryAugmentedProposal::getProposalName( void ) const
{
    static std::string name = "CharacterHistoryAugmentedProposal";
    return name;
}


double CharacterHistoryAugmentedProposal::getProposalTuningParameter( void ) const
{
    // this proposal has no tuning parameter
    return RbConstants::Double::nan;
}


void CharacterHistoryAugmentedProposal::prepareProposal( void )
{
    
}


void CharacterHistoryAugmentedProposal::printParameterSummary(std::ostream &o, bool name_only) const
{


}


void CharacterHistoryAugmentedProposal::undoProposal( void )
{
    
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old ctmc that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void CharacterHistoryAugmentedProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{

//    ctmc = static_cast< StochasticNode<AbstractHomologousDiscreteCharacterData>* >(newN) ;
//
//    ctmc_dist = dynamic_cast< StateDependentSpeciationExtinctionProcess* >( &ctmc->getDistribution() );
//    if ( ctmc_dist == NULL )
//    {
//        throw RbException("The CharacterHistoryAugmentedProposal is currently only implemented for CDBDP distributions.");
//    }
}


void CharacterHistoryAugmentedProposal::setProposalTuningParameter(double tp)
{
    // this proposal has no tuning parameter: nothing to do
}


void CharacterHistoryAugmentedProposal::tune( double rate )
{
    
    
}

