#Sets80

CLI for dealing with sets of strings less than 80 characters long.

##Usage
- `create <set name>` — Create a new empty set.
- `delete <set name>` — Delete a set.
- `view [set name]` — View all available sets or insides of a set.
- `card <set name>` — View set's cardinality.
- `add <element> <set name>` — Add element to a set.
- `remove <element> <set name>` — Remove element from a set.
- `belongs? <element> <set name>` — View if element is a member of set.
- `union <set name> <set name> <set name>` — Create a new set, which is a union of two first.
- `inters <set name> <set name> <set name>` — Create a new set, which is an intersection of two first.
- `sdiff <set name> <set name> <set name>` — Create a new set, which is a symmetric difference of two first.
- `sub <set name> <set name> <set name>` — Create a new set, which is a diffecrence between first and second.
- `contains? <set name> <set name>` — View if second set is a subset of first one.
- `quit` — quit application.
- `help` — View help.