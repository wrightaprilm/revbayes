#include <stddef.h>
#include <ostream>
#include <string>

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "CharacterHistoryAugmentedProposal.h"
#include "AbstractHomologousDiscreteCharacterData.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "MetropolisHastingsMove.h"
#include "Move_CharacterHistoryAugmented.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "TypeSpec.h"
#include "Move.h"
#include "RevObject.h"
#include "RevPtr.h"
#include "RevVariable.h"
#include "RlMove.h"
#include "StochasticNode.h"

namespace RevBayesCore { class Proposal; }
namespace RevBayesCore { class AbstractHomologousDiscreteCharacterData; }
namespace RevBayesCore { template <class valueType> class TypedDagNode; }

using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
template <class charType>
Move_CharacterHistoryAugmented<charType>::Move_CharacterHistoryAugmented() : Move()
{
    std::cout << "Here I am in Rl!" << std::endl;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the move.
 */
template <class charType>
Move_CharacterHistoryAugmented<charType>* Move_CharacterHistoryAugmented<charType>::clone(void) const
{
    
    return new Move_CharacterHistoryAugmented(*this);
}


/**
 * Create a new internal move object.
 *
 * This function simply dynamically allocates a new internal move object that is
 * associated with the variable (DAG-node). The internal move object is created by calling its
 * constructor and passing the move-parameters (the variable and other parameters) as arguments of the
 * constructor. The move constructor takes care of the proper hook-ups.
 *
 */
template<class charType>
void Move_CharacterHistoryAugmented<charType>::constructInternalObject( void )
{
    delete value;
    
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();

    RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* ctmc_tdn = NULL;
    RevBayesCore::StochasticNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* ctmc_sn = NULL;

    if ( static_cast<const RevLanguage::AbstractHomologousDiscreteCharacterData&>( ctmc->getRevObject() ).isModelObject() )
    {
        ctmc_tdn = static_cast<const RevLanguage::AbstractHomologousDiscreteCharacterData&>( ctmc->getRevObject() ).getDagNode();
        ctmc_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* >(ctmc_tdn);
    }
    else
    {
        throw RbException("mvCharacterHistoryAugmented() requires a CTMC.");
    }

    RevBayesCore::Proposal *p = new RevBayesCore::CharacterHistoryAugmentedProposal(ctmc_sn);
    value = new RevBayesCore::MetropolisHastingsMove(p,w);
    
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
template <class charType>
const std::string& Move_CharacterHistoryAugmented<charType>::getClassType(void)
{
    
    static std::string rev_type = "Move_CharacterHistoryAugmented";
    
    return rev_type;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
template <class charType>
const TypeSpec& Move_CharacterHistoryAugmented<charType>::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return rev_type_spec;
}



/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
template <class charType>
std::string Move_CharacterHistoryAugmented<charType>::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "CharacterHistoryAugmented";
    
    return c_name;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the scale move are:
 * (1) the variable which must be a time-tree.
 *
 * \return The member rules.
 */
template <class charType>
const MemberRules& Move_CharacterHistoryAugmented<charType>::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        memberRules.push_back( new ArgumentRule("ctmc", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The continuous-time Markov process to monitor.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, NULL ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rules_set = true;
    }
    
    return memberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
template <class charType>
const TypeSpec& Move_CharacterHistoryAugmented<charType>::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/**
 * Print the value for the user.
 */
template <class charType>
void Move_CharacterHistoryAugmented<charType>::printValue(std::ostream &o) const
{
    
    o << "GibbsDrawCharacterHistory(";
    if (ctmc != NULL)
    {
        o << ctmc->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
    
}


/**
 * Set a member variable.
 *
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
template <class charType>
void Move_CharacterHistoryAugmented<charType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "ctmc" )
    {
        ctmc = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
    
}




