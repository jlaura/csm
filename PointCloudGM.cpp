#define CSM_POINT_CLOUD_LIBRARY
#include <algorithm>
#include "PointCloudGM.h"

using namespace csm;

std::string PointCloudGM::getFamily() const {
	return GeometricModel::getFamily() + std::string(CSM_POINTCLOUD_FAMILY);
}

std::vector<PointCloudGM::SensorPartials> PointCloudGM::computeAllSensorPartials(const EcefCoord& groundPt, param::Set pSet, double desiredPrecision, double* achievedPrecision, WarningList* warnings) const {
	std::vector<int> paramIndices = this->getParameterSetIndices(pSet);
	std::vector<PointCloudGM::SensorPartials> partials(paramIndices.size());
	if (achievedPrecision == NULL) {
		for (std::size_t i = 0; i < paramIndices.size(); ++i)
			partials[i] = computeSensorPartials(paramIndices[i], groundPt, desiredPrecision, achievedPrecision, warnings);
	} else {
		*achievedPrecision = 0.0;
		for (std::size_t i = 0; i < paramIndices.size(); ++i) {
			double tmp;
			partials[i] = this->computeSensorPartials(paramIndices[i], groundPt, desiredPrecision, &tmp, warnings);
			*achievedPrecision = std::max(*achievedPrecision, tmp);
		}
	}
	return partials;
}

std::vector<PointCloudGM::SensorPartials> PointCloudGM::computeAllSensorPartials(const ModelCoord& modelPt, const EcefCoord& groundPt, param::Set pSet, double desiredPrecision, double* achievedPrecision, WarningList* warnings) const {
	std::vector<int> paramIndices = this->getParameterSetIndices(pSet);
	std::vector<PointCloudGM::SensorPartials> partials(paramIndices.size());
	if (achievedPrecision == NULL) {
		for (std::size_t i = 0; i < paramIndices.size(); ++i)
			partials[i] = computeSensorPartials(paramIndices[i], modelPt, groundPt, desiredPrecision, achievedPrecision, warnings);
	} else {
		*achievedPrecision = 0.0;
		for (std::size_t i = 0; i < paramIndices.size(); ++i) {
			double tmp;
			partials[i] = this->computeSensorPartials(paramIndices[i], modelPt, groundPt, desiredPrecision, &tmp, warnings);
			*achievedPrecision = std::max(*achievedPrecision, tmp);
		}
	}
	return partials;
}

