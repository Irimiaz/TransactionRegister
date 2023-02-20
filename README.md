# TransactionRegister

The basic concept of this system is an open and anonymous network of nodes, or miners, that together maintain a public registry of transactions. This register of transactions is coded in the form of a chain of blocks in which each account block contains a series of transactions, the new ones that have been collected from users.

![image](https://user-images.githubusercontent.com/94114942/220174186-b73993d8-deb3-4e1d-8af3-4330d48362b7.png)

We will define the following concepts:

* past(Node) – represents the set of blocks directly or indirectly referenced by the Node block and which must be applied before the corresponding Node block is applied.
* future(Node) – represents the set with the blocks that have direct or indirect reference to the Node block and which can only be applied after the Node block has been applied.
* anticones (Node) – represents the set with the blocks for which the order between them and the block
* tips(G) – represents the set with the blocks in the graph G that have the internal degree equal to 0 (no there are still other blocks to reference)
