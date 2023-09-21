#pragma once

#include <nanoflann.hpp>
#include <vector>

using Points = std::vector<std::vector<double>>;

/** kd-tree dynamic index
 *
 * Contains the k-d trees and other information for indexing a set of points
 * for nearest-neighbor matching.
 *
 * The class "DatasetAdaptor" must provide the following interface (can be
 * non-virtual, inlined methods):
 *
 *  \code
 *   // Must return the number of data poins
 *   size_t kdtree_get_point_count() const { ... }
 *
 *   // Must return the dim'th component of the idx'th point in the class:
 *   T kdtree_get_pt(const size_t idx, const size_t dim) const { ... }
 *
 *   // Optional bounding-box computation: return false to default to a standard
 * bbox computation loop.
 *   //   Return true if the BBOX was already computed by the class and returned
 * in "bb" so it can be avoided to redo it again.
 *   //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3
 * for point clouds) template <class BBOX> bool kdtree_get_bbox(BBOX &bb) const
 *   {
 *      bb[0].low = ...; bb[0].high = ...;  // 0th dimension limits
 *      bb[1].low = ...; bb[1].high = ...;  // 1st dimension limits
 *      ...
 *      return true;
 *   }
 *
 *  \endcode
 *
 * \tparam DatasetAdaptor The user-provided adaptor (see comments above).
 * \tparam Distance The distance metric to use: nanoflann::metric_L1,
 * nanoflann::metric_L2, nanoflann::metric_L2_Simple, etc.
 * \tparam DIM Dimensionality of data points (e.g. 3 for 3D points)
 * \tparam IndexType Type of the arguments with which the data can be
 * accessed (e.g. float, double, int64_t, T*)
 */

class KdTree {
public:
    class DatasetAdaptor {
    public:
        DatasetAdaptor(const Points& points) : points_(points) {
        }

        size_t kdtree_get_point_count() const {  // NOLINT
            return points_.size();
        }

        double kdtree_get_pt(const size_t idx, const size_t dim) const {  // NOLINT
            return dim == 0 ? points_[idx][0] : dim == 1 ? points_[idx][1] : points_[idx][2];
        }

        template <class BBOX>
        bool kdtree_get_bbox(BBOX& bb) const {  // NOLINT
            (void)bb;
            return false;
        }

    private:
        std::vector<std::vector<double>> points_;
    };

    KdTree(const Points& points) : dimensionality_(points[0].size()), dataset_adaptor_(points) {
        /**
         * KDTree constructor
         *
         * Refer to docs in README.md or online in https://github.com/jlblancoc/nanoflann
         *
         * The KD-Tree point dimension (the length of each point in the datase,
         * e.g. 3 for 3D points)
         * is determined by means of:
         *  - The \a DIM template parameter if >0 (highest priority)
         *  - Otherwise, the \a dimensionality parameter of this constructor.
         *
         * @param inputData Dataset with the input features
         * @param params Basically, the maximum leaf node size
         */

        /** Squared Euclidean distance functor (generic version, optimized for
         * high-dimensionality data sets).
         *  Corresponding distance traits: nanoflann::metric_L2
         * \tparam T Type of the elements (e.g. double, float, uint8_t)
         * \tparam _DistanceType Type of distance variables (must be signed)
         * (e.g. float, double, int64_t)
         */

        tree_ = std::make_unique<nanoflann::KDTreeSingleIndexAdaptor<
            nanoflann::L2_Adaptor<double, DatasetAdaptor>, DatasetAdaptor>>(dimensionality_,
                                                                            dataset_adaptor_);
        tree_->buildIndex();
    }

    ~KdTree() {
        tree_.reset();
    }

    // https://github.com/jlblancoc/nanoflann/blob/master/examples/vector_of_vectors_example.cpp
    size_t GetNearest(const std::vector<double>& query_pt) const {
        const size_t num_closest = 1;
        std::vector<size_t> index(num_closest);
        std::vector<double> distance(num_closest);
        nanoflann::KNNResultSet<double> result_set(num_closest);
        result_set.init(&index[0], &distance[0]);

        /**
         * Find the "num_closest" nearest neighbors to the \a query_point[0:dim-1].
         * Their indices are stored inside
         * the result object.
         *  \sa radiusSearch, findNeighbors
         * \note nChecks_IGNORED is ignored but kept for compatibility with the original
         * FLANN interface.
         * \return Number `N` of valid points in the result set. Only the first `N`
         * entries in `out_indices` and `out_distances_sq` will be valid.
         *         Return may be less than `num_closest` only if the number of elements
         *         in the tree is less than `num_closest`.
         */

        tree_->findNeighbors(result_set, &query_pt[0], nanoflann::SearchParams());
        return index[0];
    }

private:
    const size_t dimensionality_;
    const DatasetAdaptor dataset_adaptor_;
    std::unique_ptr<nanoflann::KDTreeSingleIndexAdaptor<
        nanoflann::L2_Adaptor<double, DatasetAdaptor>, DatasetAdaptor>>
        tree_ = nullptr;
};
