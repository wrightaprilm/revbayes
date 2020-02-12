#ifndef Move_CharacterHistoryAugmented_H
#define Move_CharacterHistoryAugmented_H

#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    

    class Move_CharacterHistoryAugmented : public Move {
        
    public:
        
        Move_CharacterHistoryAugmented(void);                                                                                                                    //!< Default constructor
        
        // Basic utility functions
        virtual Move_CharacterHistoryAugmented*         clone(void) const;                                                                      //!< Clone object
        void                                            constructInternalObject(void);                                                          //!< We construct the a new internal Move.
        static const std::string&                       getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                     getMoveName(void) const;                                                                //!< Get the name used for the constructor function in Rev.
        const MemberRules&                              getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                         getTypeSpec(void) const;                                                                //!< Get language type of the object
        virtual void                                    printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        RevPtr<const RevVariable>                       ctmc;
        
    };
    
}

#endif
