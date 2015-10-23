
namespace singleeyefitter{


template<typename Scalar>
class EllipseDistanceResidualFunction {
    public:
        EllipseDistanceResidualFunction(const cv::Mat& eye_image, const std::vector<cv::Point2f>& pupil_inliers, const Scalar& eye_radius, const Scalar& focal_length) :
            eye_image(eye_image), pupil_inliers(pupil_inliers), eye_radius(eye_radius), focal_length(focal_length) {}

        template <typename T>
        bool operator()(const T* const eye_param, const T* const pupil_param, T* e) const
        {
            typedef typename ad_traits<T>::scalar Const;
            Eigen::Matrix<T, 3, 1> eye_pos(eye_param[0], eye_param[1], eye_param[2]);
            Sphere<T> eye(eye_pos, T(eye_radius));
            Ellipse2D<T> pupil_ellipse(project(circleOnSphere(eye, pupil_param[0], pupil_param[1], pupil_param[2]), T(focal_length)));
            EllipseDistCalculator<T> ellipDist(pupil_ellipse);

            for (int i = 0; i < pupil_inliers.size(); ++i) {
                const cv::Point2f& inlier = pupil_inliers[i];
                e[i] = ellipDist(Const(inlier.x), Const(inlier.y));
            }

            return true;
        }
    private:
        const cv::Mat& eye_image;
        const std::vector<cv::Point2f>& pupil_inliers;
        const Scalar& eye_radius;
        const Scalar& focal_length;
};
} // namespace singleeyefitter
