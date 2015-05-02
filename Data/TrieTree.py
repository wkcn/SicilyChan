class Node:
    def __init__(self):
        self.name = ''
        self.next = [0 for i in range(26)]

class TrieTree:
    def __init__(self):
        self.root = Node()

    def insert(self,word,content):
        node = self.root
        for c in word.lower():
            id = ord(c) - ord('a')
            if id >= 26 or id < 0:
                continue
            if node.next[id] == 0:
                node.next[id] = Node()
            node = node.next[id]
        node.name = content

    def search(self,word):
        node = self.root
        for c in word.lower():
            if node.next[ord(c) - ord('a')] == 0:
                break
            node = node.next[ord(c) - ord('a')]
        try:
            return node.name
        except:
            return ''

#t = TrieTree()
#t.insert("hello","wo")
#print t.search('hello')
