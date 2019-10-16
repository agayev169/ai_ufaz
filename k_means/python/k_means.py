import numpy as np


class k_means:
    def __init__(self, x, n, iters=1000, threshold=0.0, initializer="random", verbose=True):
        """ Divides data into clusters.

        Args:
            x:           mxk numpy array containing data
            n:           number of clusters
            iters:       number of iterations
            threshold:   minimum threshold of change of centroids
            initializer: algorithm to initialize centers. May be \"random\" or \"k++\"
        """

        self.__data       = x.copy()
        self.__clusters_n = n

        if len(x.shape) != 2:
            raise AssertionError(f"Shape of data must be mxk, where m is the number of data points and k is the number of attributes of a data point")

        if initializer == "random":
            centroids = x[np.random.permutation(len(x))[:n]]
        elif initializer == "k++":
            centroids = np.empty((n, x.shape[1]))
            centroids[0] = x[np.random.randint(0, x.shape[0])]

            for i in range(1, n): # new centroids
                distances = np.empty(x.shape[0])
                for j, d in enumerate(x): # data points
                    for k in range(i): # assigned centroids
                        cur_dist = self.dist(d, centroids[k])
                        if cur_dist < distances[j]:
                            distances[j] = cur_dist
                
                distances *= 2

                centroids[i] = x[np.argmax(distances)]
        else:
            raise AssertionError(f"{initializer} is an unknown way to initialize centers")
        clusters  = np.empty(len(x))

        for _ in range(iters):
            for i, point in enumerate(x):
                min_dist = np.inf
                cluster  = np.inf

                for j, center in enumerate(centroids):
                    d = self.dist(center, point)
                    if d < min_dist:
                        min_dist = d
                        cluster = j

                clusters[i] = cluster

            new_centroids = np.empty(centroids.shape)
            max_dist = 0.0
            for j in range(len(centroids)):
                new_centroids[j] = self.get_center(x[clusters == j])
                d = self.dist(new_centroids[j], centroids[j])
                if d > max_dist:
                    max_dist = d
                    
            if max_dist < threshold:
                if verbose:
                    print("Early stopping.")
                break

            centroids = new_centroids
        
        self.__centroids = centroids


    def dist(self, x, y):
        """ Calculates distance between 2 data points

        Args:
            x: data point
            y: data point
        """

        return np.linalg.norm(x - y)


    def get_center(self, data):
        """ Center of the data

        Args:
            data: data to find the center
        """ 

        center = np.sum(data, axis=0) / data.shape[0]
        return center


    def predict(self, data):
        """ Predict the clusters that data belong to

        Args:
            data: mxk numpy array containing data
        """

        if len(data.shape) != 2:
            raise AssertionError(f"Data should be of mxk shape")

        if data.shape[1] != self.__data.shape[1]:
            raise AssertionError(f"Data should have the same number of attributes as the train data")

        results = np.empty(data.shape[0])

        for i, d in enumerate(data):
            cluster  = -1
            dist_min = np.inf
            for j, c in enumerate(self.__centroids):
                cur_dist = self.dist(d, c)
                if cur_dist < dist_min:
                    cluster = j
                    dist_min = cur_dist
            results[i] = cluster

        return results

if __name__ == "__main__":
    import time
    import matplotlib.pyplot as plt
    import pandas as pd

    data = pd.read_csv("iris_text.data", 
        names=["petal_width", "petal_length", "sepal_width", "sepal_length", "class"])

    data = data.values

    x_cols = list(range(4))
    data_x = data[:, x_cols]

    y_cols = [-1]
    data_y = data[:, y_cols]

    indexes = np.random.permutation(len(data))
    train_x = data_x[indexes[:100]]
    val_x   = data_x[indexes[100:]]

    train_y = data_y[indexes[:100]]
    val_y   = data_y[indexes[100:]]

    indexes = val_y[:, 0].argsort()
    val_x = val_x[indexes]
    val_y = val_y[indexes]

    k = 3
    k_means = k_means(train_x, k, initializer="k++", threshold=0.0000001)
    out = k_means.predict(val_x)

    print(out)
    print(val_y)

    fig, axs = plt.subplots(2, 6)

    labels_unique = np.unique(data_y)

    for label in labels_unique:
        axs[0, 0].scatter(val_x[:, 0][val_y[:, 0] == label], val_x[:, 1][val_y[:, 0] == label])
        axs[0, 1].scatter(val_x[:, 0][val_y[:, 0] == label], val_x[:, 2][val_y[:, 0] == label])
        axs[0, 2].scatter(val_x[:, 0][val_y[:, 0] == label], val_x[:, 3][val_y[:, 0] == label])
        axs[0, 3].scatter(val_x[:, 1][val_y[:, 0] == label], val_x[:, 2][val_y[:, 0] == label])
        axs[0, 4].scatter(val_x[:, 1][val_y[:, 0] == label], val_x[:, 3][val_y[:, 0] == label])
        axs[0, 5].scatter(val_x[:, 2][val_y[:, 0] == label], val_x[:, 3][val_y[:, 0] == label])

    axs[0, 0].set_xlabel("petal_width")
    axs[0, 0].set_ylabel("petal_height")
    axs[0, 1].set_xlabel("petal_width")
    axs[0, 1].set_ylabel("sepal_width")
    axs[0, 2].set_xlabel("petal_width")
    axs[0, 2].set_ylabel("sepal_height")
    axs[0, 3].set_xlabel("petal_height")
    axs[0, 3].set_ylabel("sepal_width")
    axs[0, 4].set_xlabel("petal_height")
    axs[0, 4].set_ylabel("sepal_height")
    axs[0, 5].set_xlabel("sepal_width")
    axs[0, 5].set_ylabel("sepal_height")

    for i in range(6):
        axs[0, i].set_title("real")


    labels_unique = np.unique(out)

    for label in labels_unique:
        axs[1, 0].scatter(val_x[:, 0][out == label], val_x[:, 1][out == label])
        axs[1, 1].scatter(val_x[:, 0][out == label], val_x[:, 2][out == label])
        axs[1, 2].scatter(val_x[:, 0][out == label], val_x[:, 3][out == label])
        axs[1, 3].scatter(val_x[:, 1][out == label], val_x[:, 2][out == label])
        axs[1, 4].scatter(val_x[:, 1][out == label], val_x[:, 3][out == label])
        axs[1, 5].scatter(val_x[:, 2][out == label], val_x[:, 3][out == label])

    axs[1, 0].set_xlabel("petal_width")
    axs[1, 0].set_ylabel("petal_height")
    axs[1, 1].set_xlabel("petal_width")
    axs[1, 1].set_ylabel("sepal_width")
    axs[1, 2].set_xlabel("petal_width")
    axs[1, 2].set_ylabel("sepal_height")
    axs[1, 3].set_xlabel("petal_height")
    axs[1, 3].set_ylabel("sepal_width")
    axs[1, 4].set_xlabel("petal_height")
    axs[1, 4].set_ylabel("sepal_height")
    axs[1, 5].set_xlabel("sepal_width")
    axs[1, 5].set_ylabel("sepal_height")

    for i in range(6):
        axs[1, i].set_title("predictions")

    plt.show()


    """
    begin = time.time()
    # print(k_means(10 * np.random.rand(1000, 5), 50, threshold=0.0001, initializer="k++"))
    data = 10 * np.random.rand(10000, 2)
    out = k_means(data, 3, threshold=0.0001, initializer="k++")
    print(f"With k-means++ initializer: {time.time() - begin} seconds")
    for i in range(3):
        plt.scatter(data[:, 0][out == i], data[:, 1][out == i])
    plt.show()
    

    begin = time.time()
    out = k_means(data, 3, threshold=0.0001)
    print(f"With random initializer: {time.time() - begin} seconds")
    for i in range(3):
        plt.scatter(data[:, 0][out == i], data[:, 1][out == i])
    plt.show()
    """

