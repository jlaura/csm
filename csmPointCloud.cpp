#define CSM_POINT_CLOUD_LIBRARY
#include "csmPointCloud.h"
#include "Error.h"
#include <cmath>

namespace csm
{

ModelBounds::ModelBounds()
{
}

ModelBounds::ModelBounds(const ModelCoord& center,
                         double dim0,
                         double dim1,
                         double dim2)
   :
      _center (center),
      _a0     (1.0, 0.0, 0.0),
      _a1     (0.0, 1.0, 0.0),
      _a2     (0.0, 0.0, 1.0),
      _dim0   (std::abs(dim0)),
      _dim1   (std::abs(dim1)),
      _dim2   (std::abs(dim2))
{
   // check for errors in dimensions
   if ((_dim0 <= 0.0) || (_dim1 <= 0.0) || (_dim2 <= 0.0))
   {
      throw csm::Error(csm::Error::ILLEGAL_MATH_OPERATION,
                       "ModelBounds dimension is not a positive value",
                       "ModelBounds constructor");
   }
}

ModelBounds::ModelBounds(const ModelCoord& center,
                         const ModelVector& axis0,
                         const ModelVector& axis1,
                         const ModelVector& axis2,
                         double dim0,
                         double dim1,
                         double dim2)
   :
      _center (center),
      _a0     (axis0),
      _a1     (axis1),
      _a2     (axis2),
      _dim0   (std::abs(dim0)),
      _dim1   (std::abs(dim1)),
      _dim2   (std::abs(dim2))
{
   // check for errors in dimensions
   if ((_dim0 <= 0.0) || (_dim1 <= 0.0) || (_dim2 <= 0.0))
   {
      throw csm::Error(csm::Error::ILLEGAL_MATH_OPERATION,
                       "ModelBounds dimension is not a positive value",
                       "ModelBounds constructor");
   }

   // orthonormalize _u, _v, and _w and simultaneously check for errors
   if (normalize(_a0) == 0.0)
   {
      throw csm::Error(csm::Error::ILLEGAL_MATH_OPERATION,
                       "ModelBounds axis-0 vector has zero length",
                       "ModelBounds constructor");
   }

   _a1 = orthogonalProjection(_a1, _a0);

   if (normalize(_a1) == 0.0)
   {
      throw csm::Error(csm::Error::ILLEGAL_MATH_OPERATION,
                       "ModelBounds axis-1 vector is linearly dependent on axis-0 vector",
                       "ModelBounds constructor");
   }

   _a2 = orthogonalProjection(orthogonalProjection(_a2, _a0), _a1);

   if (normalize(_a2) == 0.0)
   {
      throw csm::Error(csm::Error::ILLEGAL_MATH_OPERATION,
                       "ModelBounds axis-2 vector is linearly dependent on axis-0 and axis-1 vectors",
                       "ModelBounds constructor");
   }
}

bool ModelBounds::contains(const ModelCoord& modelCoord) const
{
   ModelVector offset(modelCoord.m0 - _center.m0,
                      modelCoord.m1 - _center.m1,
                      modelCoord.m2 - _center.m2);

   double offset0 = offset.m0 * _a0.m0 + offset.m1 * _a0.m1 + offset.m2 * _a0.m2;
   double offset1 = offset.m0 * _a1.m0 + offset.m1 * _a1.m1 + offset.m2 * _a1.m2;
   double offset2 = offset.m0 * _a2.m0 + offset.m1 * _a2.m1 + offset.m2 * _a2.m2;

   return (std::abs(offset0) <= (0.5 * _dim0)) &&
          (std::abs(offset1) <= (0.5 * _dim1)) &&
          (std::abs(offset2) <= (0.5 * _dim2));
}

double ModelBounds::normalize(ModelVector& v)
{
   double k = v.m0 * v.m0 + v.m1 * v.m1 + v.m2 * v.m2;
   if (k == 0.0)
   {
      return 0.0;
   }
   k = std::sqrt(k);
   v.m0 /= k;
   v.m1 /= k;
   v.m2 /= k;
   return k;
}

ModelVector ModelBounds::orthogonalProjection(const ModelVector &u,
                                              const ModelVector &v)
{
   double vDotV = v.m0 * v.m0 + v.m1 * v.m1 + v.m2 * v.m2;
   if (vDotV == 0.0)
   {
      return u;
   }
   double k = (u.m0 * v.m0 + u.m1 * v.m1 + u.m2 * v.m2) / vDotV;
   return ModelVector(u.m0 - k * v.m0, u.m1 - k * v.m1, u.m2 - k * v.m2);
}

} // namespace csm
