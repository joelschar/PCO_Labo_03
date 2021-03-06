/**************************************
 *
 * threadcalcul.cpp
 *
 * autheurs: Yann Lederrey, Joel Schär
 *
 * Implémentation de la classe
 * threadcalul, voir le header pour
 * une description.
 *************************************/

#include "threadcalcul.h"

void ThreadCalcul::run() {
    // yStart et yFinal définissent notre hauteur de départ
    // et hauteur de fin de travail pour le thread donné.
    // Cette boucle démarre le calcul de l'image selon les lignes.
    for (int y = yStart; y < yFinal; ++y) {
        if (*restart)
            break;
        if (*abort)
            return;

        QRgb *scanLine = reinterpret_cast<QRgb *>(image->scanLine(y + halfHeight));
        double ay = centerY + (y * scaleFactor);

        for (int x = -halfWidth; x < halfWidth; ++x) {
            double ax = centerX + (x * scaleFactor);
            double a1 = ax;
            double b1 = ay;
            int numIterations = 0;

            do {
                ++numIterations;
                double a2 = (a1 * a1) - (b1 * b1) + ax;
                double b2 = (2 * a1 * b1) + ay;
                if ((a2 * a2) + (b2 * b2) > Limit)
                    break;

                ++numIterations;
                a1 = (a2 * a2) - (b2 * b2) + ax;
                b1 = (2 * a2 * b2) + ay;
                if ((a1 * a1) + (b1 * b1) > Limit)
                    break;
            } while (numIterations < MaxIterations);

            if (numIterations < MaxIterations) {
                *scanLine++ = colormap[numIterations % ColormapSize];
            } else {
                *scanLine++ = qRgb(0, 0, 0);
            }
        }
    }
}
