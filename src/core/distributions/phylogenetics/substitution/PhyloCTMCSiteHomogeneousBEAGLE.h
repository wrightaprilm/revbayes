#ifndef PhyloCTMCSiteHomogeneousBEAGLE_H
#define PhyloCTMCSiteHomogeneousBEAGLE_H

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"

namespace RevBayesCore {

    template<class charType>
    class PhyloCTMCSiteHomogeneousBEAGLE : public AbstractPhyloCTMCSiteHomogeneous<charType> {

    public:
        PhyloCTMCSiteHomogeneousBEAGLE(const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb, bool internal, bool gapmatch);
        virtual                                            ~PhyloCTMCSiteHomogeneousBEAGLE(void);                                                                   //!< Virtual destructor

        // public member functions
        PhyloCTMCSiteHomogeneousBEAGLE*                     clone(void) const;                                                                                      //!< Create an independent clone


    protected:

        virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r);
        virtual void                                        computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
        virtual void                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        virtual void                                        computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        virtual double                                      sumRootLikelihood( void );


    private:

        double                                              ln_beagle_probability;

    };

}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"

#include <cmath>
#include <cstring>

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::PhyloCTMCSiteHomogeneousBEAGLE(const TypedDagNode<Tree> *t, size_t nChars, bool c, size_t nSites, bool amb, bool internal, bool gapmatch) : AbstractPhyloCTMCSiteHomogeneous<charType>(  t, nChars, 1, c, nSites, amb, internal, gapmatch )
{

}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::~PhyloCTMCSiteHomogeneousBEAGLE( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>* RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::clone( void ) const
{

    return new PhyloCTMCSiteHomogeneousBEAGLE<charType>( *this );
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t node_index, size_t left, size_t right)
{

    // compute the transition probability matrix
//    this->updateTransitionProbabilities( node_index );
      
    BeagleOperation b_operation;

    b_operation.destinationPartials    = (int) node_index + this->activeLikelihood[node_index]*this->num_nodes;
    b_operation.destinationScaleWrite  = BEAGLE_OP_NONE;
    b_operation.destinationScaleRead   = BEAGLE_OP_NONE;
    if ( node.getChild( 0 ).isTip() == true )
    {
        b_operation.child1Partials         = (int) left;
    }
    else
    {
        b_operation.child1Partials         = (int) left       + this->activeLikelihood[left]*this->num_nodes;
    }
    if ( node.getChild( 1 ).isTip() == true )
    {
        b_operation.child2Partials         = (int) right;
    }
    else
    {
        b_operation.child2Partials         = (int) right       + this->activeLikelihood[right]*this->num_nodes;
    }
    b_operation.child1TransitionMatrix = (int) left       + this->activeLikelihood[left]*this->num_nodes;
    b_operation.child2TransitionMatrix = (int) right      + this->activeLikelihood[right]*this->num_nodes;

    // @Daniel/Sebastian/Killian: Collect calls to BEAGLE here into global vector
    beagleUpdatePartials(this->beagle_instance, &b_operation, 1, BEAGLE_OP_NONE);

//        const double* b_tp_begin = this->transition_prob_matrices[0].theMatrix;
//        // @Daniel/Sebastian/Killian: Collect calls to BEAGLE here into global vector (separate)
//        beagleSetTransitionMatrix(this->beagle_instance, (int) node_index + this->activeLikelihood[node_index]*this->num_nodes, b_tp_begin, (double) 1.0);
        
   // TODO: this entire block of computing the branch length should probably be done in the base class as its own function
   // first, get the branch time
   double branch_time = node.getBranchLength();
   
   // second, get the clock rate for the branch
   double rate = 1.0;
   if ( this->branch_heterogeneous_clock_rates == true )
   {
       rate = this->heterogeneous_clock_rates->getValue()[node_index];
   }
   else if ( this->homogeneous_clock_rate != NULL)
   {
       rate = this->homogeneous_clock_rate->getValue();
   }
   
   double branch_length = rate * branch_time;
   
   int tmp_prob_indices = (int) node_index + this->activeLikelihood[node_index]*this->num_nodes;
   
   beagleUpdateTransitionMatrices(this->beagle_instance, this->active_eigen_system[0], &tmp_prob_indices, NULL, NULL, &branch_length, 1);



}



template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{

    // @TODO: Modify code for unrooted trees to fit here

}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle )
{
    
    // @TODO: Extend implementation to work on site mixtures
    
    // reset the likelihood
    this->lnProb = 0.0;
    
    // get the root frequencies
    std::vector<std::vector<double> > ff;
    this->getRootFrequencies(ff);

    size_t num_taxa = (this->num_nodes + 2)/2;
           
    BeagleOperation b_operation;

    b_operation.destinationPartials    = (int) root + this->activeLikelihood[root]*this->num_nodes;
    b_operation.destinationScaleWrite  = BEAGLE_OP_NONE;
    b_operation.destinationScaleRead   = BEAGLE_OP_NONE;
    b_operation.child1TransitionMatrix = (int) left       + this->activeLikelihood[left]*this->num_nodes;
    b_operation.child2TransitionMatrix = (int) right      + this->activeLikelihood[right]*this->num_nodes;
    if ( left < num_taxa )
    {
        b_operation.child1Partials         = (int) left;
    }
    else
    {
        b_operation.child1Partials         = (int) left       + this->activeLikelihood[left]*this->num_nodes;
    }
    if ( right < num_taxa )
    {
        b_operation.child2Partials         = (int) right;
    }
    else
    {
        b_operation.child2Partials         = (int) right       + this->activeLikelihood[right]*this->num_nodes;
    }
    
    // @Daniel/Sebastian/Killian: Collect calls to BEAGLE here into global vector
    beagleUpdatePartials(this->beagle_instance, &b_operation, 1, BEAGLE_OP_NONE);

    const std::vector<double> &b_f = ff[0];
    const double* b_inStateFrequencies     = &b_f[0];
          int     b_stateFrequenciesIndex  = 0;

    beagleSetStateFrequencies(this->beagle_instance,
                                  b_stateFrequenciesIndex,
                                  b_inStateFrequencies);

    int     b_parentBufferIndices     = (int) root + this->activeLikelihood[root]*this->num_nodes;
    int     b_childBufferIndices      = (int) middle + this->activeLikelihood[middle]*this->num_nodes;
    if ( middle < num_taxa )  b_childBufferIndices      = (int) middle;
    int     b_probabilityIndices      = (int) middle + this->activeLikelihood[middle]*this->num_nodes;
    int*    b_firstDerivativeIndices  = NULL;
    int*    b_secondDerivativeIndices = NULL;
    int     b_categoryWeightsIndices  = 0;
    int     b_stateFrequenciesIndices = b_stateFrequenciesIndex;
    int     b_cumulativeScaleIndices  = BEAGLE_OP_NONE;
    int     b_count                   = 1;
    double  b_outSumLogLikelihood;
    double* b_outSumFirstDerivative   = NULL;
    double* b_outSumSecondDerivative  = NULL;
    
    beagleCalculateEdgeLogLikelihoods(this->beagle_instance,
                                      &b_parentBufferIndices,
                                      &b_childBufferIndices,
                                      &b_probabilityIndices,
                                      b_firstDerivativeIndices,
                                      b_secondDerivativeIndices,
                                      &b_categoryWeightsIndices,
                                      &b_stateFrequenciesIndices,
                                      &b_cumulativeScaleIndices,
                                      b_count,
                                      &b_outSumLogLikelihood,
                                      b_outSumFirstDerivative,
                                      b_outSumSecondDerivative);
        
    this->ln_beagle_probability = b_outSumLogLikelihood;

}




template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::computeTipLikelihood(const TopologyNode &node, size_t node_index)
{

    // TODO: this entire block of computing the branch length should probably be done in the base class as its own function
    // first, get the branch time
    double branch_time = node.getBranchLength();
    
    // second, get the clock rate for the branch
    double rate = 1.0;
    if ( this->branch_heterogeneous_clock_rates == true )
    {
        rate = this->heterogeneous_clock_rates->getValue()[node_index];
    }
    else if ( this->homogeneous_clock_rate != NULL)
    {
        rate = this->homogeneous_clock_rate->getValue();
    }
    
    double branch_length = rate * branch_time;
    
    int tmp_prob_indices = (int) node_index + this->activeLikelihood[node_index]*this->num_nodes;
    
    beagleUpdateTransitionMatrices(this->beagle_instance, this->active_eigen_system[0], &tmp_prob_indices, NULL, NULL, &branch_length, 1);
    
}



template<class charType>
double RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>::sumRootLikelihood( void )
{
    return this->ln_beagle_probability;
}


#endif
