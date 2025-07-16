#pragma once

#include "CoreMinimal.h"

/**
 * A small collection of camera?projection helper functions.
 */
namespace MathHelpers
{
    /**
     * Computes a pinhole camera intrinsic matrix from a vertical FOV (degrees)
     * and image dimensions.
     *
     * @param FOV         Vertical field of view in degrees.
     * @param ImageWidth  Width of the image in pixels.
     * @param ImageHeight Height of the image in pixels.
     * @return            4×4 Intrinsic matrix.
     */
    FMatrix ComputeIntrinsicMatrix(float FOV, int ImageWidth, int ImageHeight);

    /**
     * Projects a 3D point in camera?space into 2D image coordinates using
     * the given intrinsic matrix. Returns false if the point is behind
     * the camera (X ? 0).
     *
     * @param IntrinsicMatrix      The 3×3 camera matrix (embedded in 4×4).
     * @param CameraSpacePosition  Point coordinates in camera space (X forward).
     * @param OutImageCoordinates  Returned pixel coordinates (origin at top?left).
     * @return                     True if projection succeeded (X > 0).
     */
    bool ProjectToImage(const FMatrix& IntrinsicMatrix,
        const FVector& CameraSpacePosition,
        FVector2D& OutImageCoordinates);
}
