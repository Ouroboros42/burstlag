#ifndef COMPARISON_H
#define COMPARISON_H

#include "core.hpp"
#include "caching/factorials.hpp"
#include "caching/outputs.hpp"
#include "util/pair_ops.hpp"

#include <unordered_map>
#include <utility>

/* Encodes information about relative rates of both detectors */
class DetectorRelation {
    /* Defining the parameters for detectors with expected event rates (per histogram bin):
        Detector 1 Rate = ax + b
        Detector 2 Rate = px + q
    Where x is the superova neutrino flux reaching earth. (This value, and even its units, are unimportant as we only ever use a/p).

    k is the suppression factor.
    */

    scalar_pair log_sensitivity; // = -log(1 + p/a), -log(1 + a/p)
    
    scalar_pair rate_const; // = alpha, rho = b(1 + p/a)/k, q(1 + a/p)/k
    scalar_pair log_rate_const; // = log(alpha), log(rho)

    scalar log_const_prefactor; // - (b + q) + (log(1-1/k) if k>1, else 0)

    /* Simplest constructor, directly sets attributes */
    DetectorRelation(scalar_pair log_sensitivity, scalar_pair rate_const, scalar_pair log_rate_const, scalar log_const_prefactor);

    /* Intermediate constructor, used internally.
    Defines attributes from:
        bin_background_rate = (b/k, q/k)
        sensitivity = (a/(a+p), p/(a+p))
        log_suppression_prefactor = log(1-1/k) if k>1, else 0

    */
    DetectorRelation(scalar_pair bin_background_rate, scalar_pair sensitivity, scalar log_suppression_prefactor);

    /* Cache of calculated likelihoods for reuse */
    std::unordered_map<likelihood_args, scalar, hash_args> previous_outputs;

    friend class BinSumTerms;

public:
    /* Create a detector relation for a pair of detectors with the given parameters:
    bin_background_rate_1/2 - Expected number of background events per histogram bin at detector 1/2.
    sensitivity_ratio_2_to_1 - Expected rate of supernova neutrino events at detector 2 relative to detector 1.
    source_suppression - Bayesian prior parameter, encoding how unlikely very high event counts are.
        The default value (1) means no suppression.
        Larger values mean higher event counts are assumed more unlikely.
        Must be greater than 1.
    */
    DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_ratio_2_to_1, scalar source_suppression);

    /* Initialise for identical detectors with 0 background.
    This is included only to provide a default constructor to Cython.*/
    DetectorRelation();

    /* Create equivalent DetectorRelation for the same detectors in the opposite order*/
    DetectorRelation flip();
    
    /* Returns the log_likelihood of the given observed neutrino counts to specified rel_precision.
        use_cache specifies whether to use the output cache - that remembers previous inputs and their outputs.
        fcache stores calculate factorials, and is *always* used
        rel_precision specifies the desired maximum error, relative to the value of the likelihood. 
            This corresponds approximately to the maximum absolute error of the calculated log-likelihood.
    */
    scalar bin_log_likelihood(FactorialCache& fcache, size_t count_1, size_t count_2, scalar rel_precision, bool use_cache);
};

#endif