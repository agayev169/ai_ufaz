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
    if cols == "all":
        cols = [col for col in df.columns]
    for col in cols:
        for cl in range(classes_n):
            df.iloc[:][col][cl * len(df[col]) // 3 : (cl + 1) * len(df[col]) // 3] = cl
    return df


def str_to_one_hot(df, cols="class"):
    # TODO: implement!
    pass


if __name__ == "__main__":
    data = pd.read_csv("iris_text.data", 
        names=["petal_width", "petal_length", "sepal_width", "sepal_length", "class"])
    print(data.head())
    print(data.describe())

    data = data.sort_values(by="petal_width")
    data = float_to_one_hot(data, [col for col in data.columns if col != "class"], 3)

    # dummy = pd.get_dummies(data)
    # print(f"dummy : {dummy}")

    print(data)

    x_cols = [col for col in data.columns if col != "class"]
    data_x = data.loc[:, x_cols]

    y_cols = ["class"]
    data_y = data.loc[:, y_cols]
    dt = decision_tree((data_x, data_y))
    