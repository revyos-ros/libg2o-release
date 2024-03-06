// g2o - General Graph Optimization
// Copyright (C) 2011 R. Kuemmerle, G. Grisetti, H. Strasdat, W. Burgard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "dquat2mat.h"

#include <iostream>
namespace g2o {
namespace internal {
using namespace std;

#include "dquat2mat_maxima_generated.cpp"

int _q2m(double& S, double& qw, const double& r00, const double& r10,
         const double& r20, const double& r01, const double& r11,
         const double& r21, const double& r02, const double& r12,
         const double& r22) {
  double tr = r00 + r11 + r22;
  if (tr > 0) {
    S = sqrt(tr + 1.0) * 2;  // S=4*qw
    qw = 0.25 * S;
    // qx = (r21 - r12) / S;
    // qy = (r02 - r20) / S;
    // qz = (r10 - r01) / S;
    return 0;
  } else if ((r00 > r11) & (r00 > r22)) {
    S = sqrt(1.0 + r00 - r11 - r22) * 2;  // S=4*qx
    qw = (r21 - r12) / S;
    // qx = 0.25 * S;
    // qy = (r01 + r10) / S;
    // qz = (r02 + r20) / S;
    return 1;
  } else if (r11 > r22) {
    S = sqrt(1.0 + r11 - r00 - r22) * 2;  // S=4*qy
    qw = (r02 - r20) / S;
    // qx = (r01 + r10) / S;
    // qy = 0.25 * S;
    return 2;
  } else {
    S = sqrt(1.0 + r22 - r00 - r11) * 2;  // S=4*qz
    qw = (r10 - r01) / S;
    // qx = (r02 + r20) / S;
    // qy = (r12 + r21) / S;
    // qz = 0.25 * S;
    return 3;
  }
}

void compute_dq_dR(Eigen::Matrix<double, 3, 9, Eigen::ColMajor>& dq_dR,
                   const double& r11, const double& r21, const double& r31,
                   const double& r12, const double& r22, const double& r32,
                   const double& r13, const double& r23, const double& r33) {
  double qw;
  double S;
  int whichCase = _q2m(S, qw, r11, r21, r31, r12, r22, r32, r13, r23, r33);
  S *= .25;
  switch (whichCase) {
    case 0:
      compute_dq_dR_w(dq_dR, S, r11, r21, r31, r12, r22, r32, r13, r23, r33);
      break;
    case 1:
      compute_dq_dR_x(dq_dR, S, r11, r21, r31, r12, r22, r32, r13, r23, r33);
      break;
    case 2:
      compute_dq_dR_y(dq_dR, S, r11, r21, r31, r12, r22, r32, r13, r23, r33);
      break;
    case 3:
      compute_dq_dR_z(dq_dR, S, r11, r21, r31, r12, r22, r32, r13, r23, r33);
      break;
  }
  if (qw <= 0) dq_dR *= -1;
}
}  // namespace internal
}  // namespace g2o
