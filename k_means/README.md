# K-Means

---

K-Means is an algorithm used to partition data into clusters.

The algorithm consists of the following steps:
1. Identify K initial points - centroids;
2. Associate each data point with the closest centroid;
3. Calculate the center of the data points associated to each of the centroids and identify results as new centroids;
4. Repeat steps 2-4 until clusters do not change.

To identify initial centroids one may use one of the two following approaches:
1. Randomly choose K data points;
2. Use K-Means++ algorithm:
    1. Randomly pick a data point which becomes a centroid;
    2. Find the distance between each data point and the closest cetroid and denote this as $D(x)$;
    3. Compute $D^2(x)$ and choose the point with maximum $D^2(x)$ as a new centroid.
    4. Repeat steps 2-4 until K centroids are chosen

---
## Implementations:
* [Python 3](./python/)
* [C++](./cpp/)