# Decision Tree

---

Decision tree is an algorithm that classifies data by forming a tree structure. Decision tree is made of nodes which have rules associated with them. The idea is to find an attribute of a dataset that best classifies it. When such attribute is found, a tree is splitted into several sub-trees each containing a possible value of the attribute (approach of this implementation). Then, each sub-tree is treated as tree, which means that a new attribute that now best classifies data is chosen and so on. The process of splitting finishes, when there are no attributes to use for splitting or the depth of the tree is already too big (customizable variable) and splitting a tree leads to overfitting.

To split a tree we need to have a function that would help us to decide which attribute bes classifies data. For this we need the following functions:
* Shannon Entropy
Shannon entropy is used to calculate the homogenity of an attribute, i.e. whether data is divided evenly or not.
Formula of the Shannon entropy of an attribute is given by $ H(A) = - \sum_i{p_i \log_2 p_i} $.
Formula of the Shannon entropy of two attributes is given by $ H(A, B) = - \sum_i^B{p_i * H(i)} $.
* Informaiton Gain (Discriminative Power)
Information gain is a function that defines which attribute of the dataset classifies it the best.
Formula of the information gain is given by $ G(A, B) = E(A) - E(A, B) $

---
## Implementations:
* [Python 3](./python/)