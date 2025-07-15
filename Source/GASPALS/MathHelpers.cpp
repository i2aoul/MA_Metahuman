// MathHelpers.cpp

#include "MathHelpers.h"
#include "Math/UnrealMathUtility.h"

namespace MathHelpers
{
    FMatrix ComputeIntrinsicMatrix(float FOV, int ImageWidth, int ImageHeight)
    {
        float fx = ImageWidth / (2.0f * FMath::Tan(FOV * 0.5f * (PI / 180.0f)));
        float fy = fx; // quadratische Pixel
        float cx = ImageWidth * 0.5f;
        float cy = ImageHeight * 0.5f;

        return FMatrix(
            FPlane(fx, 0, cx, 0),
            FPlane(0, fy, cy, 0),
            FPlane(0, 0, 1, 0),
            FPlane(0, 0, 0, 1)
        );
    }

    bool ProjectToImage(const FMatrix& Intr, const FVector& CameraSpacePos, FVector2D& OutImage)
    {
        if (CameraSpacePos.X <= 0)
        {
            OutImage = FVector2D::ZeroVector;
            return false;
        }

        float x = CameraSpacePos.Y / CameraSpacePos.X;
        float y = CameraSpacePos.Z / CameraSpacePos.X;

        OutImage.X = Intr.M[0][0] * x + Intr.M[0][2];
        OutImage.Y = -Intr.M[1][1] * y + Intr.M[1][2];
        return true;
    }
}
