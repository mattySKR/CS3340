# Author: Matvey Skripchenko
# Student number: 250899673

class Disjoint:

    def __init__(self):
        self.sets = []

    # Constructs an disjoint-set data type with n elements, 1, 2, . . . , n.
    def uandf(self,n):
        self.sets.append([n])

    # Creates a new set whose only member (and thus representative) is i.
    def make_set(self,i):
        self.sets = []
        self.sets.append(i)

    # Unites the dynamic sets that contains i and j, respectively, into a new
    # set that is the union of these two sets.
    def union_sets(self,i,j):
        s1 = self.find_sets(i)
        s2 = self.find_sets(j)
        if s1 != s2:
            s1.extend(s2)
            self.sets.remove(s2)

    # Returns the representative of the set containing i.
    def find_sets(self,i):
        for target in self.sets:
            if i in target:
                return target

    # Returns the total number of sets
    def final_sets(self):
        return self.sets


