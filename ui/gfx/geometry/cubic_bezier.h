// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GEOMETRY_CUBIC_BEZIER_H_
#define UI_GFX_GEOMETRY_CUBIC_BEZIER_H_

#include "base/macros.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

class GFX_EXPORT CubicBezier {
 public:
  CubicBezier(double p1x, double p1y, double p2x, double p2y);

  double SampleCurveX(double t) const {
    // `ax t^3 + bx t^2 + cx t' expanded using Horner's rule.
    return ((ax_ * t + bx_) * t + cx_) * t;
  }

  double SampleCurveY(double t) const {
    return ((ay_ * t + by_) * t + cy_) * t;
  }

  double SampleCurveDerivativeX(double t) const {
    return (3.0 * ax_ * t + 2.0 * bx_) * t + cx_;
  }

  double SampleCurveDerivativeY(double t) const {
    return (3.0 * ay_ * t + 2.0 * by_) * t + cy_;
  }

  // Given an x value, find a parametric value it came from.
  // x must be in [0, 1] range. Doesn't use gradients.
  double SolveCurveX(double x, double epsilon) const;

  // Evaluates y at the given x with default epsilon.
  double Solve(double x) const;
  // Evaluates y at the given x. The epsilon parameter provides a hint as to the
  // required accuracy and is not guaranteed. Uses gradients if x is
  // out of [0, 1] range.
  double SolveWithEpsilon(double x, double epsilon) const {
    if (x < 0.0)
      return 0.0 + start_gradient_ * x;
    if (x > 1.0)
      return 1.0 + end_gradient_ * (x - 1.0);
    return SampleCurveY(SolveCurveX(x, epsilon));
  }

  // Returns an approximation of dy/dx at the given x with default epsilon.
  double Slope(double x) const;
  // Returns an approximation of dy/dx at the given x.
  // Clamps x to range [0, 1].
  double SlopeWithEpsilon(double x, double epsilon) const;

  // Sets |min| and |max| to the bezier's minimum and maximium y values in the
  // interval [0, 1].
  // TODO(loyso): Implement this as two independent getters.
  void Range(double* min, double* max) const {
    *min = range_min_;
    *max = range_max_;
  }

 private:
  void InitCoefficients(double p1x, double p1y, double p2x, double p2y);
  void InitGradients(double p1x, double p1y, double p2x, double p2y);
  void InitRange(double p1y, double p2y);

  double ax_;
  double bx_;
  double cx_;

  double ay_;
  double by_;
  double cy_;

  double start_gradient_;
  double end_gradient_;

  double range_min_;
  double range_max_;

  DISALLOW_ASSIGN(CubicBezier);
};

}  // namespace gfx

#endif  // UI_GFX_GEOMETRY_CUBIC_BEZIER_H_
