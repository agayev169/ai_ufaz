import pandas as pd
import numpy as np

class decision_tree():
    class __rule():
        def __init__(self, col, val):
            """
            A rule of a node of decision tree

            Args:
                col: column that defines the rule
                val: value that defines the rule
            """
            self.__col = col
            self.__val = val

        
        def is_satisfied(self, data):
            """
                Checks whether rule is satisfied given a data point

                Args:
                    data: data point of shape 1xm

            Returns:
                bool - whether rule is satisfied or not
            """
            if self.__col == None and self.__val == None:
                return True
            
            return data[0, self.__col] == self.__val
        
        def __str__(self):
            """
                Returns rule as a string
            """
            return f"col: {self.__col}\nval: {self.__val}"


    def __init__(self, data, max_depth=5, indexes="all", cols_exclude=[], rule=None):
        """
        Decision Tree

        Args:
            data:          tupple containing x of shape nxm and y of shape kxm
            max_depth:   maximum depth of the tree
            indexes:     indexes of the data that are associated with the decision tree. Either "all" or a numpy array of shape n
            cols_exlude: columns that are excluded
            rule:        rule that defines the decision tree
        """
        self.__rule_ = rule
        self.__max_depth = max_depth
        self.__data = data
        self.__branches = []
        self.__cols_exclude = cols_exclude
        if isinstance(indexes, str) and indexes == "all":
            self.__indexes = np.ones(len(data[0]), dtype=bool)
        else:
            self.__indexes = indexes.copy()
        
        if max_depth > 0:
            self.__branches = self.__branch()

    
    def get_rule(self):
        """
        Returns the rule that defines the decision tree
        """
        return self.__rule_


    def __branch(self):
        """
        Creates a new branch based on discriminative power of columns

        Returns:
            new decision tree
        """
        if len(np.unique(self.__data[1][self.__indexes])) <= 1:
            return []

        branches     = []
        disc_max     = -np.inf
        disc_max_col = None

        for col in range(self.__data[0].shape[1]):
            if col in self.__cols_exclude:
                continue
            disc = self.disc(col)
            if disc > disc_max:
                disc_max     = disc
                disc_max_col = col

        if disc_max_col == None:
            return branches
            
        uniques = np.unique(self.__data[0][self.__indexes, disc_max_col])
        cols_exclude = self.__cols_exclude
        cols_exclude.append(disc_max_col)
        for unique in uniques:
            indexes = (self.__data[0][:, disc_max_col] == unique)
            indexes = np.logical_and(self.__indexes, indexes)
            rule = self.__rule(disc_max_col, unique)
            branches.append(decision_tree(self.__data, self.__max_depth - 1, indexes, cols_exclude, rule))
            
        return branches
    

    def entropy(self, col=None, val=None):
        """
        Calculates entropy of a dataset based on column and value or the dataset itself

        Args:
            col: column to determine the entropy of. Calculates entropy of the dataset by default
            val: value of the col column to calculate the entropy of

        Returns:
            entropy as float
        """
        res = 0
        if col == None:
            uniques, counts = np.unique(self.__data[1][self.__indexes], return_counts=True)
            for val, count in zip(uniques, counts):
                if len(self.__data[1][self.__indexes] == val) != 0:
                    res -= (count / len(self.__data[1][self.__indexes]) * np.log2(count / len(self.__data[1][self.__indexes])))
        else:
            uniques_y, counts_y = np.unique(self.__data[1][self.__indexes], return_counts=True)
            for unique_y, count_y in zip(uniques_y, counts_y):
                y = self.__data[0][np.logical_and(self.__indexes, (self.__data[1] == [unique_y]).reshape(-1))]
                y = y[:, col][y[:, col] == val]
                if len(y) != 0:
                    res -= (len(y) / count_y * np.log2(len(y) / count_y))

        return res

    
    def disc(self, col):
        """
        Calculates discriminative power of the variable

        Args:
            col: the column to calculate the discriminative power of

        Returns:
            discriminative power as float
        """
        res = self.entropy()
        
        uniques, counts = np.unique(self.__data[0][self.__indexes, col], return_counts=True)
        for val, count in zip(uniques, counts):
            res -= ((count / len(self.__data[0][self.__indexes, col])) * self.entropy(col, val))
        
        return res

    
    def predict(self, data_x):
        """
        Predicts the class of a data point

        Args:
            data_x: data point of shape 1xm

        Returns:
            an integer - corresponding class
        """
        if len(self.__branches) == 0:
            vals, counts = np.unique(self.__data[1][self.__indexes], return_counts=True)
            return vals[np.argmax(counts)]
        else:
            corresponding_branch = None
            for branch in self.__branches:
                if branch.get_rule().is_satisfied(data_x):
                    corresponding_branch = branch
                    break
            if corresponding_branch == None:
                for branch in self.__branches:
                    print(branch.get_rule())
                return 0
            return corresponding_branch.predict(data_x)


def float_to_one_hot(data, cols="all", classes_n=3):
    """
    Encodes float data as one-hot

    Args:
        data:      numpy array of shape nxm 
        cols:      "all" or list of columns
        classes_n: number of classes to have in resulting one-hot array

    Returns:
        one-hot encoded numpy array
    """
    if cols != "all" and not isinstance(cols, list):
        raise AssertionError("cols must be either \"all\" or a list of names of columns")
    if cols == "all":
        cols = [col for col in range(data.shape[1])]
    data = data.copy()
    for col in cols:
        data[:, col] = np.sort(data[:, col])
        for cl in range(classes_n):
            data[cl * len(data) // classes_n : (cl + 1) * len(data) // classes_n, col] = cl
    return data


def str_to_one_hot(data, cols=[-1]):
    """
    Encodes string data as one-hot

    Args:
        data: numpy array of shape nxm 
        cols: "all" or list of columns

    Returns:
        one-hot encoded numpy array
    """
    if cols != "all" and not isinstance(cols, list):
        raise AssertionError("cols must be either \"all\" or a list of names of columns")
    if cols == "all":
        cols = [col for col in data.columns]
    data = data.copy()
    dictionary = np.unique(data[:, cols])
    for col1 in cols:
        for i, col2 in enumerate(dictionary):
            data[data[:, col1] == col2, col1] = i
    return data, dictionary


if __name__ == "__main__":
    data = pd.read_csv("iris_text.data", 
        names=["petal_width", "petal_length", "sepal_width", "sepal_length", "class"])

    data = data.values

    data = float_to_one_hot(data, [0, 1, 2, 3], 10)
    data, d = str_to_one_hot(data, [-1])

    x_cols = [i for i in range(4)]
    data_x = data[:, x_cols]

    y_cols = [-1]
    data_y = data[:, y_cols]


    indexes = np.random.permutation(len(data))
    train_x = data_x[indexes[:100]]
    val_x   = data_x[indexes[100:]]

    train_y = data_y[indexes[:100]]
    val_y   = data_y[indexes[100:]]

    dt = decision_tree((train_x, train_y), 5)

    acc = 0
    for i in range(len(val_x)):
        pred = dt.predict(val_x[i].reshape(1, -1))
        if pred == val_y[i, 0]:
            acc += 1
        else:
            print(f"val_x[{i}]: {val_x[i]}")

    print(f"accuracy: {acc / len(val_x)}")