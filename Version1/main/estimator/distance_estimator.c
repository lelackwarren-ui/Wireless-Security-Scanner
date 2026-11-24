#include "distance_estimator.h"
#include <math.h>

/*
 * Estimation indicative basée sur le modèle de perte de propagation.
 * tx_power_at_1m : puissance RSSI théorique à 1 mètre.
 * path_loss : facteur d'environnement (2 = espace ouvert approximatif).
 * Les murs, obstacles, antennes et interférences rendent cette estimation
 * très imprécise. Elle ne doit pas être interprétée comme une mesure réelle.
 */
float distance_estimate_m(int rssi)
{
    const float tx_power_at_1m = -59.0f;
    const float path_loss = 2.0f;
    return powf(10.0f, ((tx_power_at_1m - (float)rssi) /
                        (10.0f * path_loss)));
}