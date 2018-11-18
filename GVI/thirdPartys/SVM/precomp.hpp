/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_ML_PRECOMP_HPP__
#define __OPENCV_ML_PRECOMP_HPP__

#include "opencv2/core.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/core/utility.hpp"

#define __OPENCV_BUILD

#include "opencv2/cvconfig.h"

#ifdef HAVE_EIGEN
#  if defined __GNUC__ && defined __APPLE__
#    pragma GCC diagnostic ignored "-Wshadow"
#  endif
#  include <Eigen/Core>
#  include "opencv2/core/eigen.hpp"
#endif

#ifdef HAVE_TBB
#  include "tbb/tbb_stddef.h"
#  if TBB_VERSION_MAJOR*100 + TBB_VERSION_MINOR >= 202
#    include "tbb/tbb.h"
#    include "tbb/task.h"
#    undef min
#    undef max
#  else
#    undef HAVE_TBB
#  endif
#endif

//! @cond IGNORED

namespace cv
{
#ifdef HAVE_TBB

    typedef tbb::blocked_range<int> BlockedRange;

    template<typename Body> static inline
        void parallel_for(const BlockedRange& range, const Body& body)
    {
        tbb::parallel_for(range, body);
    }

    typedef tbb::split Split;

    template<typename Body> static inline
        void parallel_reduce(const BlockedRange& range, Body& body)
    {
        tbb::parallel_reduce(range, body);
    }

    typedef tbb::concurrent_vector<Rect> ConcurrentRectVector;
#else
    class BlockedRange
    {
    public:
        BlockedRange() : _begin(0), _end(0), _grainsize(0) {}
        BlockedRange(int b, int e, int g = 1) : _begin(b), _end(e), _grainsize(g) {}
        int begin() const { return _begin; }
        int end() const { return _end; }
        int grainsize() const { return _grainsize; }

    protected:
        int _begin, _end, _grainsize;
    };

    template<typename Body> static inline
        void parallel_for(const BlockedRange& range, const Body& body)
    {
        body(range);
    }
    typedef std::vector<Rect> ConcurrentRectVector;

    class Split {};

    template<typename Body> static inline
        void parallel_reduce(const BlockedRange& range, Body& body)
    {
        body(range);
    }
#endif

    // Returns a static string if there is a parallel framework,
    // NULL otherwise.
    CV_EXPORTS const char* currentParallelFramework();
} //namespace cv

/****************************************************************************************\
*                                  Common declarations                                   *
\****************************************************************************************/

/* the alignment of all the allocated buffers */
#define  CV_MALLOC_ALIGN    16

/* IEEE754 constants and macros */
#define  CV_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))
#define  CV_TOGGLE_DBL(x) ((x)^((int64)(x) < 0 ? CV_BIG_INT(0x7fffffffffffffff) : 0))

static inline void* cvAlignPtr(const void* ptr, int align = 32)
{
    CV_DbgAssert((align & (align - 1)) == 0);
    return (void*)(((size_t)ptr + align - 1) & ~(size_t)(align - 1));
}

static inline int cvAlign(int size, int align)
{
    CV_DbgAssert((align & (align - 1)) == 0 && size < INT_MAX);
    return (size + align - 1) & -align;
}

#ifdef IPL_DEPTH_8U
static inline cv::Size cvGetMatSize(const CvMat* mat)
{
    return cv::Size(mat->cols, mat->rows);
}
#endif

namespace cv
{
    CV_EXPORTS void scalarToRawData(const cv::Scalar& s, void* buf, int type, int unroll_to = 0);
}

// property implementation macros

#define CV_IMPL_PROPERTY_RO(type, name, member) \
	inline type get##name() const { return member; }

#define CV_HELP_IMPL_PROPERTY(r_type, w_type, name, member) \
	CV_IMPL_PROPERTY_RO(r_type, name, member) \
	inline void set##name(w_type val) { member = val; }

#define CV_HELP_WRAP_PROPERTY(r_type, w_type, name, internal_name, internal_obj) \
	r_type get##name() const { return internal_obj.get##internal_name(); } \
	void set##name(w_type val) { internal_obj.set##internal_name(val); }

#define CV_IMPL_PROPERTY(type, name, member) CV_HELP_IMPL_PROPERTY(type, type, name, member)
#define CV_IMPL_PROPERTY_S(type, name, member) CV_HELP_IMPL_PROPERTY(type, const type &, name, member)

#define CV_WRAP_PROPERTY(type, name, internal_name, internal_obj)  CV_HELP_WRAP_PROPERTY(type, type, name, internal_name, internal_obj)
#define CV_WRAP_PROPERTY_S(type, name, internal_name, internal_obj) CV_HELP_WRAP_PROPERTY(type, const type &, name, internal_name, internal_obj)

#define CV_WRAP_SAME_PROPERTY(type, name, internal_obj) CV_WRAP_PROPERTY(type, name, name, internal_obj)
#define CV_WRAP_SAME_PROPERTY_S(type, name, internal_obj) CV_WRAP_PROPERTY_S(type, name, name, internal_obj)

#define CV_IPP_CHECK_COND (cv::ipp::useIPP())
#define CV_IPP_CHECK() if(CV_IPP_CHECK_COND)

#ifndef IPPI_CALL
#  define IPPI_CALL(func) CV_Assert((func) >= 0)
#endif

/* IPP-compatible return codes */
typedef enum CvStatus
{
    CV_BADMEMBLOCK_ERR = -113,
    CV_INPLACE_NOT_SUPPORTED_ERR = -112,
    CV_UNMATCHED_ROI_ERR = -111,
    CV_NOTFOUND_ERR = -110,
    CV_BADCONVERGENCE_ERR = -109,

    CV_BADDEPTH_ERR = -107,
    CV_BADROI_ERR = -106,
    CV_BADHEADER_ERR = -105,
    CV_UNMATCHED_FORMATS_ERR = -104,
    CV_UNSUPPORTED_COI_ERR = -103,
    CV_UNSUPPORTED_CHANNELS_ERR = -102,
    CV_UNSUPPORTED_DEPTH_ERR = -101,
    CV_UNSUPPORTED_FORMAT_ERR = -100,

    CV_BADARG_ERR = -49,  //ipp comp
    CV_NOTDEFINED_ERR = -48,  //ipp comp

    CV_BADCHANNELS_ERR = -47,  //ipp comp
    CV_BADRANGE_ERR = -44,  //ipp comp
    CV_BADSTEP_ERR = -29,  //ipp comp

    CV_BADFLAG_ERR = -12,
    CV_DIV_BY_ZERO_ERR = -11, //ipp comp
    CV_BADCOEF_ERR = -10,

    CV_BADFACTOR_ERR = -7,
    CV_BADPOINT_ERR = -6,
    CV_BADSCALE_ERR = -4,
    CV_OUTOFMEM_ERR = -3,
    CV_NULLPTR_ERR = -2,
    CV_BADSIZE_ERR = -1,
    CV_NO_ERR = 0,
    CV_OK = CV_NO_ERR
}
CvStatus;

#ifdef HAVE_TEGRA_OPTIMIZATION
namespace tegra {
    CV_EXPORTS bool useTegra();
    CV_EXPORTS void setUseTegra(bool flag);
}
#endif

//! @endcond

#undef __OPENCV_BUILD

//#include "opencv2/core.hpp"

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <vector>

/****************************************************************************************\
 *                               Main struct definitions                                  *
 \****************************************************************************************/

 /* log(2*PI) */
#define CV_LOG2PI (1.8378770664093454835606594728112)

namespace cv
{
    namespace ml
    {
        using std::vector;

#define CV_DTREE_CAT_DIR(idx,subset) \
        (2*((subset[(idx)>>5]&(1 << ((idx) & 31)))==0)-1)

        template<typename _Tp> struct cmp_lt_idx
        {
            cmp_lt_idx(const _Tp* _arr) : arr(_arr) {}
            bool operator ()(int a, int b) const { return arr[a] < arr[b]; }
            const _Tp* arr;
        };

        template<typename _Tp> struct cmp_lt_ptr
        {
            cmp_lt_ptr() {}
            bool operator ()(const _Tp* a, const _Tp* b) const { return *a < *b; }
        };

        static inline void setRangeVector(std::vector<int>& vec, int n)
        {
            vec.resize(n);
            for (int i = 0; i < n; i++)
                vec[i] = i;
        }

        static inline void writeTermCrit(FileStorage& fs, const TermCriteria& termCrit)
        {
            if ((termCrit.type & TermCriteria::EPS) != 0)
                fs << "epsilon" << termCrit.epsilon;
            if ((termCrit.type & TermCriteria::COUNT) != 0)
                fs << "iterations" << termCrit.maxCount;
        }

        static inline TermCriteria readTermCrit(const FileNode& fn)
        {
            TermCriteria termCrit;
            double epsilon = (double)fn["epsilon"];
            if (epsilon > 0)
            {
                termCrit.type |= TermCriteria::EPS;
                termCrit.epsilon = epsilon;
            }
            int iters = (int)fn["iterations"];
            if (iters > 0)
            {
                termCrit.type |= TermCriteria::COUNT;
                termCrit.maxCount = iters;
            }
            return termCrit;
        }

        struct TreeParams
        {
            TreeParams();
            TreeParams(int maxDepth, int minSampleCount,
                double regressionAccuracy, bool useSurrogates,
                int maxCategories, int CVFolds,
                bool use1SERule, bool truncatePrunedTree,
                const Mat& priors);

            inline void setMaxCategories(int val)
            {
                if (val < 2)
                    CV_Error(CV_StsOutOfRange, "max_categories should be >= 2");
                maxCategories = std::min(val, 15);
            }
            inline void setMaxDepth(int val)
            {
                if (val < 0)
                    CV_Error(CV_StsOutOfRange, "max_depth should be >= 0");
                maxDepth = std::min(val, 25);
            }
            inline void setMinSampleCount(int val)
            {
                minSampleCount = std::max(val, 1);
            }
            inline void setCVFolds(int val)
            {
                if (val < 0)
                    CV_Error(CV_StsOutOfRange,
                        "params.CVFolds should be =0 (the tree is not pruned) "
                        "or n>0 (tree is pruned using n-fold cross-validation)");
                if (val == 1)
                    val = 0;
                CVFolds = val;
            }
            inline void setRegressionAccuracy(float val)
            {
                if (val < 0)
                    CV_Error(CV_StsOutOfRange, "params.regression_accuracy should be >= 0");
                regressionAccuracy = val;
            }

            inline int getMaxCategories() const { return maxCategories; }
            inline int getMaxDepth() const { return maxDepth; }
            inline int getMinSampleCount() const { return minSampleCount; }
            inline int getCVFolds() const { return CVFolds; }
            inline float getRegressionAccuracy() const { return regressionAccuracy; }

            CV_IMPL_PROPERTY(bool, UseSurrogates, useSurrogates)
                CV_IMPL_PROPERTY(bool, Use1SERule, use1SERule)
                CV_IMPL_PROPERTY(bool, TruncatePrunedTree, truncatePrunedTree)
                CV_IMPL_PROPERTY_S(cv::Mat, Priors, priors)

        public:
            bool  useSurrogates;
            bool  use1SERule;
            bool  truncatePrunedTree;
            Mat priors;

        protected:
            int   maxCategories;
            int   maxDepth;
            int   minSampleCount;
            int   CVFolds;
            float regressionAccuracy;
        };

        struct RTreeParams
        {
            RTreeParams();
            RTreeParams(bool calcVarImportance, int nactiveVars, TermCriteria termCrit);
            bool calcVarImportance;
            int nactiveVars;
            TermCriteria termCrit;
        };

        struct BoostTreeParams
        {
            BoostTreeParams();
            BoostTreeParams(int boostType, int weakCount, double weightTrimRate);
            int boostType;
            int weakCount;
            double weightTrimRate;
        };

        class DTreesImpl : public DTrees
        {
        public:
            struct WNode
            {
                WNode()
                {
                    class_idx = sample_count = depth = complexity = 0;
                    parent = left = right = split = defaultDir = -1;
                    Tn = INT_MAX;
                    value = maxlr = alpha = node_risk = tree_risk = tree_error = 0.;
                }

                int class_idx;
                double Tn;
                double value;

                int parent;
                int left;
                int right;
                int defaultDir;

                int split;

                int sample_count;
                int depth;
                double maxlr;

                // global pruning data
                int complexity;
                double alpha;
                double node_risk, tree_risk, tree_error;
            };

            struct WSplit
            {
                WSplit()
                {
                    varIdx = next = 0;
                    inversed = false;
                    quality = c = 0.f;
                    subsetOfs = -1;
                }

                int varIdx;
                bool inversed;
                float quality;
                int next;
                float c;
                int subsetOfs;
            };

            struct WorkData
            {
                WorkData(const Ptr<TrainData>& _data);

                Ptr<TrainData> data;
                vector<WNode> wnodes;
                vector<WSplit> wsplits;
                vector<int> wsubsets;
                vector<double> cv_Tn;
                vector<double> cv_node_risk;
                vector<double> cv_node_error;
                vector<int> cv_labels;
                vector<double> sample_weights;
                vector<int> cat_responses;
                vector<double> ord_responses;
                vector<int> sidx;
                int maxSubsetSize;
            };

            CV_WRAP_SAME_PROPERTY(int, MaxCategories, params)
                CV_WRAP_SAME_PROPERTY(int, MaxDepth, params)
                CV_WRAP_SAME_PROPERTY(int, MinSampleCount, params)
                CV_WRAP_SAME_PROPERTY(int, CVFolds, params)
                CV_WRAP_SAME_PROPERTY(bool, UseSurrogates, params)
                CV_WRAP_SAME_PROPERTY(bool, Use1SERule, params)
                CV_WRAP_SAME_PROPERTY(bool, TruncatePrunedTree, params)
                CV_WRAP_SAME_PROPERTY(float, RegressionAccuracy, params)
                CV_WRAP_SAME_PROPERTY_S(cv::Mat, Priors, params)

                DTreesImpl();
            virtual ~DTreesImpl();
            virtual void clear();

            String getDefaultName() const { return "opencv_ml_dtree"; }
            bool isTrained() const { return !roots.empty(); }
            bool isClassifier() const { return _isClassifier; }
            int getVarCount() const { return varType.empty() ? 0 : (int)(varType.size() - 1); }
            int getCatCount(int vi) const { return catOfs[vi][1] - catOfs[vi][0]; }
            int getSubsetSize(int vi) const { return (getCatCount(vi) + 31) / 32; }

            virtual void setDParams(const TreeParams& _params);
            virtual void startTraining(const Ptr<TrainData>& trainData, int flags);
            virtual void endTraining();
            virtual void initCompVarIdx();
            virtual bool train(const Ptr<TrainData>& trainData, int flags);

            virtual int addTree(const vector<int>& sidx);
            virtual int addNodeAndTrySplit(int parent, const vector<int>& sidx);
            virtual const vector<int>& getActiveVars();
            virtual int findBestSplit(const vector<int>& _sidx);
            virtual void calcValue(int nidx, const vector<int>& _sidx);

            virtual WSplit findSplitOrdClass(int vi, const vector<int>& _sidx, double initQuality);

            // simple k-means, slightly modified to take into account the "weight" (L1-norm) of each vector.
            virtual void clusterCategories(const double* vectors, int n, int m, double* csums, int k, int* labels);
            virtual WSplit findSplitCatClass(int vi, const vector<int>& _sidx, double initQuality, int* subset);

            virtual WSplit findSplitOrdReg(int vi, const vector<int>& _sidx, double initQuality);
            virtual WSplit findSplitCatReg(int vi, const vector<int>& _sidx, double initQuality, int* subset);

            virtual int calcDir(int splitidx, const vector<int>& _sidx, vector<int>& _sleft, vector<int>& _sright);
            virtual int pruneCV(int root);

            virtual double updateTreeRNC(int root, double T, int fold);
            virtual bool cutTree(int root, double T, int fold, double min_alpha);
            virtual float predictTrees(const Range& range, const Mat& sample, int flags) const;
            virtual float predict(InputArray inputs, OutputArray outputs, int flags) const;

            virtual void writeTrainingParams(FileStorage& fs) const;
            virtual void writeParams(FileStorage& fs) const;
            virtual void writeSplit(FileStorage& fs, int splitidx) const;
            virtual void writeNode(FileStorage& fs, int nidx, int depth) const;
            virtual void writeTree(FileStorage& fs, int root) const;
            virtual void write(FileStorage& fs) const;

            virtual void readParams(const FileNode& fn);
            virtual int readSplit(const FileNode& fn);
            virtual int readNode(const FileNode& fn);
            virtual int readTree(const FileNode& fn);
            virtual void read(const FileNode& fn);

            virtual const std::vector<int>& getRoots() const { return roots; }
            virtual const std::vector<Node>& getNodes() const { return nodes; }
            virtual const std::vector<Split>& getSplits() const { return splits; }
            virtual const std::vector<int>& getSubsets() const { return subsets; }

            TreeParams params;

            vector<int> varIdx;
            vector<int> compVarIdx;
            vector<uchar> varType;
            vector<Vec2i> catOfs;
            vector<int> catMap;
            vector<int> roots;
            vector<Node> nodes;
            vector<Split> splits;
            vector<int> subsets;
            vector<int> classLabels;
            vector<float> missingSubst;
            vector<int> varMapping;
            bool _isClassifier;

            Ptr<WorkData> w;
        };

        template <typename T>
        static inline void readVectorOrMat(const FileNode & node, std::vector<T> & v)
        {
            if (node.type() == FileNode::MAP)
            {
                Mat m;
                node >> m;
                m.copyTo(v);
            }
            else if (node.type() == FileNode::SEQ)
            {
                node >> v;
            }
        }
    }
}

#endif /* __OPENCV_ML_PRECOMP_HPP__ */