#ifndef RandomNumberGenerator_H
#define RandomNumberGenerator_H

#include <stdint.h>
#include <chrono>
#include "RbConstants.h"

namespace RevBayesCore {

    /**
    * @brief
    * ### Pseudo Random Number Generator
    *
    * This class provides a pseudo random number generator using the xoshiro256** algorithm.
    *
    * The code for this class was adapted from the pure C implementation by David Blackman
    * and Sebastiano Vigna in the paper: *Scrambled Linear Pseudorandom Number Generators*.
    *
    * The original C implementation provided by Blackman and Vigna is licenced as public
    * domain. See <http://creativecommons.org/publicdomain/zero/1.0/>.
    *
    * More information can be found at http://prng.di.unimi.it.
    *
    */

    class RandomNumberGenerator {

        public:

            RandomNumberGenerator   ( void );         //!< Default constructor using time seed

            uint64_t    getSeed     ( void ) const;   //!< Get the seed values
            void        setSeed     ( uint64_t x );   //!< Set the seeds of the RNG
            double      uniform01   ( void );         //!< Get a random [0,1) var

        private:

            uint64_t    seed;                         //!< Seed for PRNG
            uint64_t    state[4];                     //!< Internal state vector of PRNG

            void        updateState ( void );         //!< Update the internal state based on seed
            uint64_t    next        ( void );         //!< Return next pseudo random number

    };

}

#endif //-- RandomNumberGenerator.h


//-- Anonymous namespace for private methods
namespace
{
    //-- Rotate operation for xoshiro generator internal use
    static inline uint64_t
    rotl( const uint64_t x, int k )
    {
        return (x << k) | (x >> (64 - k));
    }

    //-- Split-mix random generator used to seed the xoshiro generator
    uint64_t
    splitmix64_next ( uint64_t x )
    {
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z          = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z          = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
}

/**
 * Default constructor.
 * The default constructor allocating the object and sets the initial seed using
 * the current time.
 */
RevBayesCore::RandomNumberGenerator::RandomNumberGenerator( void )
{
    this->seed = static_cast<uint64_t>(std::chrono::high_resolution_clock::now()
                                      .time_since_epoch()
                                      .count());
    this->seed = this->seed % RbConstants::Integer::max;
    this->updateState();
}


/**
 * Internal method to update the xoshiro state array based on the current seed.
 */
void
RevBayesCore::RandomNumberGenerator::updateState ( void )
{
    this->state[0] = splitmix64_next(this->seed);
    this->state[1] = splitmix64_next(this->state[0]);
    this->state[2] = splitmix64_next(this->state[1]);
    this->state[3] = splitmix64_next(this->state[2]);
}


/**
 * Retrieve the current seed from the generator.
 *
 * \return    The current seed value.
 */
uint64_t
RevBayesCore::RandomNumberGenerator::getSeed ( void ) const
{
    return this->seed;
}


/**
 * Update the generators current seed value, and correspondingly update the internal
 * state array.
 *
 * \param[in]   x   The new seed value.
 */
void
RevBayesCore::RandomNumberGenerator::setSeed ( uint64_t x )
{
    this->seed = x;
    this->updateState();
}


/**
 * Retrieve the next pseudo random value in the sequence.
 *
 * This method also mutates the internal state to retrieve the next value.
 *
 * \return    The next psedo random number.
 */
uint64_t
RevBayesCore::RandomNumberGenerator::next ( void )
{
    const uint64_t result = rotl(this->state[1] * 5, 7) * 9;
    const uint64_t t      = this->state[1] << 17;

    this->state[2] ^= this->state[0];
    this->state[3] ^= this->state[1];
    this->state[1] ^= this->state[2];
    this->state[0] ^= this->state[3];

    this->state[2] ^= t;

    this->state[3] = rotl(this->state[3], 45);

    return result;
}


/**
 * Generate a random number from the Uniform distribution,
 * @f$ x ~ \textit{U}(0,1) @f$.
 *
 * \return    A pseudo random number in range [0,1)..
 */
double
RevBayesCore::RandomNumberGenerator::uniform01 ( void )
{
    return 0;
//    return (this->next() >> 11) * 0x1.0p-53;
}



