#!/bin/python3

import math
import os
import random
import re
import sys

class SinglyLinkedListNode:
    def __init__(self, node_data):
        self.data = node_data
        self.next = None

class SinglyLinkedList:
    def __init__(self):
        self.head = None
        self.tail = None

    def insert_node(self, node_data):
        node = SinglyLinkedListNode(node_data)

        if not self.head:
            self.head = node
        else:
            self.tail.next = node


        self.tail = node

def print_singly_linked_list(node, sep, fptr):
    while node:
        fptr.write(str(node.data))

        node = node.next

        if node:
            fptr.write(sep)

#
# Complete the 'reverse' function below.
#
# The function is expected to return an INTEGER_SINGLY_LINKED_LIST.
# The function accepts INTEGER_SINGLY_LINKED_LIST llist as parameter.
#

#
# For your reference:
#
# SinglyLinkedListNode:
#     int data
#     SinglyLinkedListNode next
#
#

def reverse(llist):
    # Write your code here
    curr_node = llist
    head = None
    while curr_node is not None:
        pre_node = curr_node.next
        curr_node.next = head
        head = curr_node
        curr_node = pre_node
    return head
    # Example process:
    # Initial list: head = (1) -> 2 -> 3 -> None
    # Reversed list building process:
    #
    # Before Loop:             head = (None),                curr_node = 1
    # While-loop:
    # Loop-1: pre_node = 2,    head = (1) -> None,           curr_node: 1 => 2
    # Loop-2: pre_node = 3,    head = (2) -> 1 -> None,      curr_node: 2 => 3
    # Loop-3: pre_node = None, head = (3) -> 2 -> 1 -> None, curr_node: 3 => None
    #
    # Reversed list: head = (3) -> 2 -> 1 -> None

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    tests = int(input())

    for tests_itr in range(tests):
        llist_count = int(input())

        llist = SinglyLinkedList()

        for _ in range(llist_count):
            llist_item = int(input())
            llist.insert_node(llist_item)

        llist1 = reverse(llist.head)

        print_singly_linked_list(llist1, ' ', fptr)
        fptr.write('\n')

    fptr.close()
