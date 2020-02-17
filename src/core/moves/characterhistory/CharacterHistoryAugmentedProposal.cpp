#include "CharacterHistoryAugmentedProposal.h"

#include <stddef.h>
#include <typeinfo>
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
        Proposal(), ctmc( n ),     include_simmaps( true ),
        use_simmap_default( true ),
        index(0)
{

    // add the node to the base class
    addNode( ctmc );

    std::cout << "We are in the core!" << std::endl;
    std::cout << "ctmc = " << ctmc << std::endl;
    std::cout << "ctmc_dist = " << ctmc_dist << std::endl;
    std::cout << "ctmc->getDagNodeType() = " << ctmc->getDistribution() << std::endl;
    std::cout << "ctmc->isClamped() = " << ctmc->isClamped() << std::endl;
    std::cout << typeid(ctmc).name() << std::endl;
    AbstractPhyloCTMCSiteHomogeneous<RnaState>* x1 = dynamic_cast<AbstractPhyloCTMCSiteHomogeneous<RnaState>* >( &ctmc->getDistribution() );
    std::cout << "x1 = " << x1 << std::endl;
    AbstractPhyloCTMCSiteHomogeneous<DnaState>* x = dynamic_cast<AbstractPhyloCTMCSiteHomogeneous<DnaState>* >( &ctmc->getDistribution() );
    std::cout << "x = " << x << std::endl;
    std::cout << x->getTree() << std::endl;
    //std::cout << x->getTemplateState().getDataType() << std::endl;
    
    std::cout << x->getTree()->getValue() << std::endl;
    
    std::cout << ctmc->getDagNodeType() << std::endl;
    //std::cout <<         ctmc->template_state  << std::endl;
    //ctmc_dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<RevBayesCore::AbstractHomologousDiscreteCharacterData* >>(ctmc->getDistribution());
    std::cout << ctmc->getValue().getNumberOfTaxa() << std::endl;
    std::cout << ctmc->getValue().getNumberOfCharacters() << std::endl;
    //std::cout << this->getDistribution() << std::endl;
    //ctmc_dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<AbstractHomologousDiscreteCharacterData>* >( &ctmc->getDistribution() );

    /*ctmc_dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<AbstractHomologousDiscreteCharacterData>* >( &ctmc->getDistribution() );
    tree = const_cast<TypedDagNode<Tree>* >( ctmc_dist->getTree() );

    size_t num_sites = ctmc_dist->getValue().getNumberOfCharacters();
    size_t num_nodes = tree->getValue().getNumberOfNodes();
    std::cout << "in core 2" << std::endl; //I got it to work up to here. Next - try some functionality of PhyloCTMC

    std::vector<std::vector<AbstractHomologousDiscreteCharacterData> > startStates(num_nodes,std::vector<AbstractHomologousDiscreteCharacterData>(num_sites));
    std::vector<std::vector<AbstractHomologousDiscreteCharacterData> > endStates(num_nodes,std::vector<AbstractHomologousDiscreteCharacterData>(num_sites));

    // draw ancestral states
    std::cout << "in core 3" << std::endl;
    ctmc_dist->drawJointConditionalAncestralStates(startStates, endStates);
    std::cout << "in core 4" << std::endl;*/



    /*std::vector<std::string> character_histories( num_nodes );
    std::cout << "in core 3" << std::endl;
    // draw stochastic character map
    ctmc_dist->drawStochasticCharacterMap(character_histories, index, use_simmap_default );
    std::cout << "I am in the core 4" << std::endl;
    // print out
    const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
    std::cout << "I am in the core 5" << std::endl;
    for (int i = 0; i < nds.size(); i++)
    {

        size_t node_index = nds[i]->getIndex();

        // add a separator before every new element
        std::cout << ";" ;

        // print out this branch's character history in the format
        // used by SIMMAP and phytools
        std::cout << character_histories[ node_index ];

    }
    std::cout << "I am in the core 6" << std::endl;*/



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

