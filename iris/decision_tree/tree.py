import pandas as pd

class decision_tree():
    def __init__(self, data, max_depth=5):
        """
        Decision Tree

        Args:
            data: tupple containing x and y
            max_depth: maximum depth of the tree
        """
        self.__max_depth = max_depth


class Node():
    def __init__(self):
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
    print(data.head())
    print(data.describe())
    
    data = float_to_one_hot(data, [col for col in data.columns if col != "class"], 3)
    data, d = str_to_one_hot(data)
    
    print(data, d)

    x_cols = [col for col in data.columns if col != "class"]
    data_x = data.loc[:, x_cols]

    y_cols = ["class"]
    data_y = data.loc[:, y_cols]
    dt = decision_tree((data_x, data_y))
    