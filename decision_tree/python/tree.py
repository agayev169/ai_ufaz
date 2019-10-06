import pandas as pd
import numpy as np

class decision_tree():
    def __init__(self, df, max_depth=5, indexes=[], cols_exclude=[]):
        """
        Decision Tree

        Args:
            df:        tupple containing x and y
            max_depth: maximum depth of the tree
        """
        self.__max_depth = max_depth
        self.__df = df
        self.__branches = []
        self.__cols_exclude = cols_exclude
        if len(indexes) == 0:
            self.__indexes = np.ones(len(df[0]), dtype=bool)
        else:
            self.__indexes = indexes.copy()
        print(f"depth: {5 - max_depth}")
        print(self.__indexes)
        
        if max_depth > 0:
            self.__branches = self.__branch()


    def __branch(self):
        branches     = []
        disc_max     = -np.inf
        disc_max_col = None

        for col in self.__df[0].columns:
            if col in self.__cols_exclude:
                continue
            disc = self.disc(col)
            if disc > disc_max:
                disc_max     = disc
                disc_max_col = col
            
        if disc_max_col == None:
            return branches
            
        uniques = np.unique(self.__df[0][disc_max_col][self.__indexes].values)
        cols_exclude = self.__cols_exclude
        cols_exclude.append(disc_max_col)
        for unique in uniques:
            indexes = (self.__df[0][disc_max_col] == unique).values
            indexes = np.logical_and(indexes, self.__indexes)
            branches.append(decision_tree(self.__df, self.__max_depth - 1, indexes, cols_exclude))
            
        return branches
    

    def entropy(self, col=None, val=None):
        """
        Calculates entropy of a dataset based on column and value or the dataset itself

        Args:
            col: str, default None. column to determine the entropy of. Calculates entropy of the dataset by default
            val: value of the col column to calculate the entropy of
        """
        res = 0
        if col == None:
            # print(len(self.__indexes))
            # print(len(self.__df[0]))
            uniques, counts = np.unique(self.__df[1][self.__indexes], return_counts=True)
            for val, count in zip(uniques, counts):
                if len(self.__df[1][self.__indexes] == val) != 0:
                    res -= (count / len(self.__df[1][self.__indexes]) * np.log2(count / len(self.__df[1][self.__indexes])))
        else:
            uniques_y, counts_y = np.unique(self.__df[1][self.__indexes], return_counts=True)
            for unique_y, count_y in zip(uniques_y, counts_y):
                y = self.__df[0][self.__indexes][(self.__df[1][self.__indexes] == unique_y).values]
                y = y[col][y[col] == val]
                if len(y) != 0:
                    res -= (len(y) / count_y * np.log2(len(y) / count_y))

        return res

    
    def disc(self, col):
        """
        Calculates discriminative power of the variable

        Args:
            col: the name of the variable to calculate the discriminative power of
        """
        res = self.entropy()
        
        uniques, counts = np.unique(self.__df[0][col][self.__indexes], return_counts=True)
        for val, count in zip(uniques, counts):
            res -= ((count / len(self.__df[0][col][self.__indexes])) * self.entropy(col, val))
        
        return res

    
    def predict(self, df_x):
        # TODO: implement the function
        pass


def float_to_one_hot(df, cols="all", classes_n=3):
    """
    Makes float data one-hot

    Args:
        df: Pandas Dataframe obejct
        cols: "all" or list of name of columns
        classes_n: number of classes to have in resulting one-hot Dataframe
    """
    if cols != "all" and not isinstance(cols, list):
        raise AssertionError("cols must be either \"all\" or a list of names of columns")
    if cols == "all":
        cols = [col for col in df.columns]
    df = df.copy()
    for col in cols:
        for cl in range(classes_n):
            df = df.sort_values(by=col)
            df.iloc[:][col][cl * len(df[col]) // 3 : (cl + 1) * len(df[col]) // 3] = cl
    return df.sort_index()


def str_to_one_hot(df, cols=["class"]):
    """
    Makes string data one-hot

    Args:
        df: Pandas Dataframe obejct
        cols: "all" or list of name of columns
    """
    if cols != "all" and not isinstance(cols, list):
        raise AssertionError("cols must be either \"all\" or a list of names of columns")
    if cols == "all":
        cols = [col for col in df.columns]
    df = df.copy()
    dictionary = [k for k in pd.get_dummies(df[cols], prefix="", prefix_sep="").keys()]
    for col1 in cols:
        for i, col2 in enumerate(dictionary):
            df.loc[df[col1] == col2, col1] = i
    return df, dictionary


if __name__ == "__main__":
    data = pd.read_csv("iris_text.data", 
        names=["petal_width", "petal_length", "sepal_width", "sepal_length", "class"])
    # print(data.head())
    # print(data.describe())
    
    data = float_to_one_hot(data, [col for col in data.columns if col != "class"], 3)
    data, d = str_to_one_hot(data)

    # print(data[np.ones(len(data), dtype=bool)])
    
    # print(data, d)

    x_cols = [col for col in data.columns if col != "class"]
    data_x = data.loc[:, x_cols]

    y_cols = ["class"]
    data_y = data.loc[:, y_cols]
    dt = decision_tree((data_x, data_y))

    # for col in data_x.columns:
    #     print(f"{col}: {dt.disc(col)}")