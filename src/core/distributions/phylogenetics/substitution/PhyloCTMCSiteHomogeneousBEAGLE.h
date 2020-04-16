#ifndef PhyloCTMCSiteHomogeneousBEAGLE_H
#define PhyloCTMCSiteHomogeneousBEAGLE_H

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"


namespace RevBayesCore
{
    template<class charType>
    class PhyloCTMCSiteHomogeneousBEAGLE : public AbstractPhyloCTMCSiteHomogeneous<charType>
    {

        public:
            // Constructors
            PhyloCTMCSiteHomogeneousBEAGLE ( const TypedDagNode<Tree>* t
                                           , size_t nChars
                                           , bool c
                                           , size_t nSites
                                           , bool amb
                                           , bool internal
                                           , bool gapmatch
                                           );

            // Destructor
            virtual ~PhyloCTMCSiteHomogeneousBEAGLE ( void );                                                                   //!< Virtual destructor

            // Public Methods
            PhyloCTMCSiteHomogeneousBEAGLE* clone ( void ) const;                                                                                      //!< Create an independent clone


        protected:
            // Protected Methods
            virtual void   computeRootLikelihood         ( size_t root
                                                         , size_t l
                                                         , size_t r
                                                         );
            virtual void   computeRootLikelihood         ( size_t root
                                                         , size_t l
                                                         , size_t r
                                                         , size_t m
                                                         );
            virtual void   computeInternalNodeLikelihood ( const TopologyNode &n
                                                         , size_t nIdx
                                                         , size_t l
                                                         , size_t r
                                                         );
            virtual void   computeTipLikelihood          ( const TopologyNode &node
                                                         , size_t nIdx
                                                         );
            virtual double sumRootLikelihood             ( void );


        private:

            // Private Variables
            double ln_beagle_probability;

            // Private Methods
            double calculateBranchLength ( const TopologyNode &node, size_t node_index );

    };
}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"

#include <cmath>
#include <cstring>


//-- Try to keep a clean(ish) file structure... Refer to the namespace of this class as 'This'.
//   Remember to '#undef This' at the end of header file and to never have '#include'
//   statements after this line.
#define This RevBayesCore::PhyloCTMCSiteHomogeneousBEAGLE<charType>



template<class charType>
This::PhyloCTMCSiteHomogeneousBEAGLE ( const TypedDagNode<Tree>* t
                                     , size_t nChars
                                     , bool c
                                     , size_t nSites
                                     , bool amb
                                     , bool internal
                                     , bool gapmatch
                                     )
    : AbstractPhyloCTMCSiteHomogeneous<charType> ( t
                                                 , nChars
                                                 , 1
                                                 , c
                                                 , nSites
                                                 , amb
                                                 , internal
                                                 , gapmatch
                                                 )
{

}



template<class charType>
This::~PhyloCTMCSiteHomogeneousBEAGLE ( void )
{
    // We don't delete the parameters, because they might be used somewhere else too.
    // The model needs to do that!
}



template<class charType>
This* This::clone ( void ) const
{
    return new PhyloCTMCSiteHomogeneousBEAGLE<charType>(*this);
}


//-- @TODO: move this to the abstract class
template<class charType>
double This::calculateBranchLength ( const TopologyNode &node, size_t node_index )
{
    double branch_time = node.getBranchLength();
    double rate = 1.0;
    if ( this->branch_heterogeneous_clock_rates == true )
    {
        rate = this->heterogeneous_clock_rates->getValue()[node_index];
    }
    else if ( this->homogeneous_clock_rate != NULL)
    {
        rate = this->homogeneous_clock_rate->getValue();
    }
    return rate * branch_time;
}



template<class charType>
void This::computeInternalNodeLikelihood ( const TopologyNode &node
                                         , size_t node_index
                                         , size_t left
                                         , size_t right
                                         )
{
    //-- Work on fixing indexes here --

    //-- option 1
    //size_t node_idx  = node_index;
    //size_t left_idx  = left;
    //size_t right_idx = right;

    //-- option 2
    //size_t node_idx  = node_index + this->num_nodes * this->activeLikelihood[node_index];
    //size_t left_idx  = left       + this->num_nodes * this->activeLikelihood[left];
    //size_t right_idx = right      + this->num_nodes * this->activeLikelihood[right];

    //-- option 3
    size_t node_idx  = node_index + this->num_nodes * this->activeLikelihood[node_index];
    size_t left_idx  = (node.getChild(0).isTip()) ? left
                     : left + this->num_nodes * this->activeLikelihood[left];
    size_t right_idx = (node.getChild(1).isTip()) ? right
                     : right + this->num_nodes * this->activeLikelihood[right];

    //-- End index work --

    BeagleOperation b_operation;
    b_operation.destinationPartials    = node_idx;
    b_operation.destinationScaleWrite  = BEAGLE_OP_NONE;
    b_operation.destinationScaleRead   = BEAGLE_OP_NONE;
    b_operation.child1Partials         = left_idx;
    b_operation.child2Partials         = right_idx;
    b_operation.child1TransitionMatrix = left_idx;
    b_operation.child2TransitionMatrix = right_idx;

    double branch_length = calculateBranchLength(node, node_index);

    this->b_ops.push_back(b_operation);
    this->b_branch_lengths.push_back(branch_length);
    this->b_node_indices.push_back(node_idx);
}



template<class charType>
void This::computeRootLikelihood ( size_t root, size_t left, size_t right )
{
    // @TODO: Modify code for unrooted trees to fit here
}



template<class charType>
void This::computeRootLikelihood ( size_t root, size_t left, size_t right, size_t middle )
{
    // Reset the likelihood
    this->lnProb = 0.0;


    //-- Prepare environment to calculate likelihood

    //-- Work on fixing indexes here --
    //-- option 1
    //size_t root_idx  = root;
    //size_t mid_idx   = middle;
    //size_t left_idx  = left;
    //size_t right_idx = right;

    //-- option 2
    //size_t root_idx  = root   + this->num_nodes * this->activeLikelihood[root];
    //size_t mid_idx   = middle + this->num_nodes * this->activeLikelihood[middle];
    //size_t left_idx  = left   + this->num_nodes * this->activeLikelihood[left];
    //size_t right_idx = right  + this->num_nodes * this->activeLikelihood[right];

    //-- option 3
    size_t num_taxa  = (this->num_nodes + 2) / 2;
    size_t root_idx  = root   + this->num_nodes * this->activeLikelihood[root];
    size_t mid_idx   = middle + this->num_nodes * this->activeLikelihood[middle];
    size_t left_idx  = (left < num_taxa) ? left
                     : left + this->num_nodes * this->activeLikelihood[left];
    size_t right_idx = (right < num_taxa) ? right
                     : right + this->num_nodes * this->activeLikelihood[right];
    //-- End index work --

    int     b_parentBufferIndices     = root_idx;
    int     b_childBufferIndices      = mid_idx;
    int     b_probabilityIndices      = mid_idx;
    int*    b_firstDerivativeIndices  = NULL;
    int*    b_secondDerivativeIndices = NULL;
    int     b_cumulativeScaleIndices  = BEAGLE_OP_NONE;
    int     b_count                   = 1;
    double  b_outSumLogLikelihood     = 0;
    double* b_outSumFirstDerivative   = NULL;
    double* b_outSumSecondDerivative  = NULL;


    //-- Push the last operation onto the queue
    BeagleOperation b_operation;
    b_operation.destinationScaleWrite  = BEAGLE_OP_NONE;
    b_operation.destinationScaleRead   = BEAGLE_OP_NONE;
    b_operation.destinationPartials    = root_idx;
    b_operation.child1TransitionMatrix = left_idx;
    b_operation.child2TransitionMatrix = right_idx;
    b_operation.child1Partials         = left_idx;
    b_operation.child2Partials         = right_idx;
    this->b_ops.push_back(b_operation);

    // get and flatten the root frequencies
    std::vector<std::vector<double>> ff; this->getRootFrequencies(ff);
    //this->b_inStateFrequencies = std::vector<double>(ff.size() * ff[0].size());
    //for ( size_t i = 0; i < ff.size(); ++i )
    //{
    //    for ( size_t j = 0; j < ff[i].size(); ++j )
    //    {
    //        this->b_inStateFrequencies[i*j+j] = ff[i][j];
    //    }
    //}
    this->b_inStateFrequencies = ff[0];

    this->b_stateFrequenciesIndex = 0;
    beagleSetStateFrequencies( this->beagle_instance
                             , this->b_stateFrequenciesIndex
                             , &this->b_inStateFrequencies[0]
                             );


    EigenSystem*        eigen_system;
    size_t              model_idx;
    std::vector<double> my_eigen_values;
    std::vector<double> flat_eigen_vectors;
    std::vector<double> flat_inv_eigen_vectors;

    if ( this->num_site_mixtures < 2 )
    {
        eigen_system = this->homogeneous_rate_matrix->getValue().getEigenSystem();
        model_idx    = 0 + this->active_eigen_system[0] * this->num_site_mixtures;

        my_eigen_values        = eigen_system->getRealEigenvalues();
        flat_eigen_vectors     = eigen_system->getEigenvectors().flattenMatrix();
        flat_inv_eigen_vectors = eigen_system->getInverseEigenvectors().flattenMatrix();

        beagleSetEigenDecomposition( this->beagle_instance
                                   , model_idx
                                   , &flat_eigen_vectors[0]
                                   , &flat_inv_eigen_vectors[0]
                                   , &my_eigen_values[0]
                                   );

        this->b_model_indices.push_back(model_idx);
    }
    else
    {
        for ( size_t i = 0; i < this->num_site_mixtures; ++i )
        {
            eigen_system = this->heterogeneous_rate_matrices->getValue()[i].getEigenSystem();
            model_idx    = i + this->active_eigen_system[i] * this->num_site_mixtures;

            my_eigen_values        = eigen_system->getRealEigenvalues();
            flat_eigen_vectors     = eigen_system->getEigenvectors().flattenMatrix();
            flat_inv_eigen_vectors = eigen_system->getInverseEigenvectors().flattenMatrix();

            beagleSetEigenDecomposition( this->beagle_instance
                                       , model_idx
                                       , &flat_eigen_vectors[0]
                                       , &flat_inv_eigen_vectors[0]
                                       , &my_eigen_values[0]
                                       );

            this->b_model_indices.push_back(model_idx);
        }
    }

    //-- Calculate and update all transition matrix buffers
    beagleUpdateTransitionMatricesWithMultipleModels( this->beagle_instance
                                                    , &this->b_model_indices[0]
                                                    , &this->b_categoryWeightsIndices
                                                    , &b_probabilityIndices
                                                    , b_firstDerivativeIndices
                                                    , b_secondDerivativeIndices
                                                    , &this->b_branch_lengths[0]
                                                    , b_count
                                                    );

    //-- Calculate and update all partial likelihood buffers
    beagleUpdatePartials( this->beagle_instance
                        , &this->b_ops[0]
                        , this->b_ops.size()
                        , BEAGLE_OP_NONE
                        );

    //-- Calclulate the lnLikelihood of the model
    beagleCalculateEdgeLogLikelihoods( this->beagle_instance
                                     , &b_parentBufferIndices
                                     , &b_childBufferIndices
                                     , &b_probabilityIndices
                                     , b_firstDerivativeIndices
                                     , b_secondDerivativeIndices
                                     , &this->b_categoryWeightsIndices
                                     , &this->b_stateFrequenciesIndex
                                     , &b_cumulativeScaleIndices
                                     , b_count
                                     , &b_outSumLogLikelihood
                                     , b_outSumFirstDerivative
                                     , b_outSumSecondDerivative
                                     );

    //-- Store the resulting likelihood
    this->ln_beagle_probability = b_outSumLogLikelihood;

    // reset the beagle operations queue
    this->b_ops.clear();
    this->b_branch_lengths.clear();
    this->b_node_indices.clear();
}



template<class charType>
void This::computeTipLikelihood ( const TopologyNode &node, size_t node_index )
{
    double branch_length = calculateBranchLength(node, node_index);

    //-- Work on fixing indexes here --

    //-- option 1
    size_t node_idx = node_index;

    //-- option 2
    //size_t node_idx = node_index + this->num_nodes * this->activeLikelihood[node_index];

    //-- End index work --

    this->b_branch_lengths.push_back(branch_length);
    this->b_node_indices.push_back(node_idx);
}



template<class charType>
double This::sumRootLikelihood ( void )
{
    return this->ln_beagle_probability;
}



//-- Undefine the local namespace shortcut
#undef This

#endif
