#include <functional>
#include <algorithm>
#include <unordered_set>
#include <random>

template <typename ContainerType, typename Ty = typename ContainerType::value_type, Ty Blank = Ty(0), typename MatchFnTy = std::function<bool(Ty, Ty)>>
class SearchStrategy
{
private:

    std::vector<uint32_t> hashes;

public:

    SearchStrategy() {};


    uint32_t fnv1a(const ContainerType &Seq)
    {
        uint32_t hash = 2166136261;
        int len = Seq.size();

        for (int i = 0; i < len; i++)
        {
            hash ^= Seq[i];
            hash *= 1099511628211;
        }

        return hash;
    }

    uint32_t fnv1a(const ContainerType &Seq, uint32_t newHash)
    {
        uint32_t hash = newHash;
        int len = Seq.size();

        for (int i = 0; i < len; i++)
        {
            hash ^= Seq[i];
            hash *= 1099511628211;
        }

        return hash;
    }

    template<uint32_t K>
    std::vector<uint32_t>& generateShinglesSingleHashPipelineTurbo(const ContainerType &Seq, uint32_t nHashes, std::vector<uint32_t> &ret)
    {
        uint32_t pipeline[K] = { 0 };
        int len = Seq.size();

        std::unordered_set<uint32_t> set;
        //set.reserve(nHashes);
        uint32_t last = 0;



        for (int i = 0; i < len; i++)
        {

            for (int k = 0; k < K; k++)
            {
                pipeline[k] ^= Seq[i];
                pipeline[k] *= 1099511628211;
            }

            //Collect head of pipeline
            if (last <= nHashes-1)
            {
                ret[last++] = pipeline[0];
                
                if (last > nHashes - 1)
                {
                    std::make_heap(ret.begin(), ret.end());
                    std::sort_heap(ret.begin(), ret.end());
                }
            }

            if (pipeline[0] < ret.front() && last > nHashes-1)
            {
               if (set.find(pipeline[0]) == set.end())
                {
                    set.insert(pipeline[0]);

                    ret[last] = pipeline[0];

                    std::sort_heap(ret.begin(), ret.end());
                }
            }

            //Shift pipeline
            for (int k = 0; k < K - 1; k++)
            {
                pipeline[k] = pipeline[k + 1];
            }
            pipeline[K - 1] = 2166136261;
        }

        return ret;
    }

    template<uint32_t K>
    std::vector<uint32_t>& generateShinglesMultipleHashPipelineTurbo(const ContainerType& Seq, uint32_t nHashes, std::vector<uint32_t>& ret, std::vector<uint32_t>& ranHash)
    {
        uint32_t pipeline[K] = { 0 };
        int len = Seq.size();

        uint32_t smallest = std::numeric_limits<uint32_t>::max();

        std::vector<uint32_t> shingleHashes(len);

        // Pipeline to hash all shingles using fnv1a
        // Store all hashes
        // While storing smallest
        // Then for each shingle hash, rehash with an XOR of 32 bit random number and store smallest
        // Do this nHashes-1 times to obtain nHashes minHashes quickly
        // Sort the hashes at the end

        for (int i = 0; i < len; i++)
        {
            for (int k = 0; k < K; k++)
            {
                pipeline[k] ^= Seq[i];
                pipeline[k] *= 1099511628211;
            }

            //Collect head of pipeline
            if (pipeline[0] < smallest)
            {
                smallest = pipeline[0];
            }
            shingleHashes[i] = pipeline[0];

            //Shift pipeline
            for (int k = 0; k < K - 1; k++)
            {
                pipeline[k] = pipeline[k + 1];
            }
            pipeline[K - 1] = 2166136261;
        }

        ret[0] = smallest;

        // Now for each hash function, rehash each shingle and store the smallest each time
        for (int i = 0; i < ranHash.size(); i++)
        {
            smallest = std::numeric_limits<uint32_t>::max();

            for (int j = 0; j < shingleHashes.size(); j++)
            {
                uint32_t temp = shingleHashes[j] ^ ranHash[i];
                
                if (temp < smallest)
                {
                    smallest = temp;
                }
            }

            ret[i+1] = smallest;
        }

        std::sort(ret.begin(), ret.end());

        return ret;
    }

    constexpr std::vector<uint32_t>& generateRandomHashFunctions(int num, std::vector<uint32_t>& ret)
    {
        //std::random_device rd;
        //std::mt19937 gen(rd());
        std::mt19937 gen(0);

        std::uniform_real_distribution<> distribution(0, std::numeric_limits<uint32_t>::max());

        //generating a random integer:
        for (int i = 0; i < num; i++)
        {
            ret[i] = distribution(gen);
        }
        return ret;
    }

    std::vector<uint32_t>& generateBands(const std::vector<uint32_t> &minHashes, uint32_t rows, uint32_t bands, std::vector<uint32_t> &lsh)
    {
        // Generate a hash for each band
        for (int i = 0; i < bands; i++)
        {
            // Perform fnv1a on the rows
            auto first = minHashes.begin() + (i*rows);
            auto last = minHashes.begin() + (i*rows) + rows;
            lsh[i] = fnv1a(std::vector<uint32_t>{first, last});
        }

        return lsh;
    }

};
