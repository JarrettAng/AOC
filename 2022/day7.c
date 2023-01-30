#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct File { 
    char name[16];
    unsigned int size;

    struct File *next;
} File;

typedef struct DirNode {
    char name[16];
    File *file;

    struct DirNode *parent;
    struct DirNode *child;
    struct DirNode *next;
} DirNode;

DirNode root = { .name = "root" };

// Clean up after we're done like good people
void free_directory_tree(void) {
    int total = 0;
    DirNode *to_remove[100]; // Pointers to all the blocks of memory we need to free

    // Loop through the tree and get all the malloc'ed nodes to free
    DirNode *current = &root;
    while (current) {
        if (current->child) {
            current = current->child;
        } else if (current->next) {
            current = current->next;
        } else {
            while (current->parent && !current->parent->next) {
                current = current->parent;
            }
            if (!current->parent) break;
            current = current->parent->next;
        }

        to_remove[total++] = current;
    }
    
    // Now we free
    for (int index = 0; index < total; ++index) {
        File *curr_file = to_remove[index]->file, *next_file;
        while (curr_file) {
            next_file = curr_file->next;
            free(curr_file);
            curr_file = next_file;
        }
        free(to_remove[index]);
    }

    root.child = NULL;
}

// Print all the information of literally everything in the root directory to stdout
void print_directory_tree (void) {
    fprintf(stdout, "DIRECTORY TREE\n");

    int tree_level = 0;
    DirNode *current = &root; // Start from the root

    // While there is still a node left in the tree we have not touched, touch them
    while (current) {
        for (int level = 0; level < tree_level; ++level) fprintf(stdout, "  ");
        fprintf(stdout, "%s\n", current->name);

        File *curr_file = current->file;
        while (curr_file)  {
            for (int level = 0; level < tree_level; ++level) fprintf(stdout, "  ");
            fprintf(stdout, "- %s (size: %u)\n", curr_file->name, curr_file->size);
            curr_file = curr_file->next;
        }

        // fprintf(stdout, "NODE %s: %s %s %s\n", current->name, current->parent->name, current->next->name, current->child->name);

        if (current->child) {
            current = current->child;
            ++tree_level;
        } else if (current->next) {
            current = current->next;
        } else {
            while (current->parent && !current->parent->next) {
                current = current->parent;
                --tree_level;
            }
            if (!current->parent) break;
            --tree_level;
            current = current->parent->next;
        }
    }
}

// Miracle of life, birth a directory into existence and add it to the tree
void create_directory(DirNode *parent, DirNode *prev, char *name) {
    DirNode *new = malloc(sizeof(DirNode));

    if (prev) {
        prev->next = new;
    } else {
        parent->child = new;
    }

    strcpy(new->name, name);
    new->parent = parent;
    new->file = NULL;
}

// File factory, create a file and return a pointer to it
File* create_file(char *name, unsigned int size) {
    File *to_store = malloc(sizeof(File));

    strcpy(to_store->name, name);
    to_store->size = size;
    to_store->next = NULL;

    return to_store;
}

// You can travel the world, as long as it exists within the directory, if not stderr that s**t
DirNode* change_directory(DirNode *current, char *next) {
    if (next[0] == '/') return &root;
    if (next[0] == '.') return current->parent;

    DirNode *current_child = current->child;
    while (current_child) {
        if(!strcmp(next, current_child->name)) {
            return current_child;
        }
        current_child = current_child->next;
    }

    fprintf(stderr, "cd failed. Target directory doesn't exist, %s -> %s\n", current->name, next);
    return current;
}

// Create the tree of life using the commands given in the file
void setup_directory_tree (FILE *ifs) {
    rewind(ifs);

    DirNode *current = &root;
    DirNode *prev;

    char input[32];

    // While there are still instructions in the file to do
    while (fgets(input, 32, ifs)) {
        char name[16] = { '\0' };
        unsigned int size = 0;
        switch(input[0]) {
            case '$': 
            if (input[2] == 'c') {
                sscanf(input, "$ cd %15s", name);
                current = change_directory(current, name);
            }
            break;

            case 'd': 
            sscanf(input, "dir %15s", name);
            prev = current->child;
            if (prev) {
                while (prev->next) {
                prev = prev->next;
                }
                create_directory(current, prev, name);
            } else {
                create_directory(current, NULL, name);
            }
            break;

            default: 
            sscanf(input, "%u %15s", &size, name);
            File *new_file = create_file( name, size); // I give up so I hardcode this part
            if (!current->file) {
                current->file = new_file;
            } else if (!current->file->next) {
                current->file->next = new_file;
            } else if (!current->file->next->next) {
                current->file->next->next = new_file;
            } else if (!current->file->next->next->next) {
                current->file->next->next->next = new_file;
            } else if (!current->file->next->next->next->next) {
                current->file->next->next->next->next = new_file;
            } else if (!current->file->next->next->next->next->next) {
                current->file->next->next->next->next->next = new_file;
            } else if (!current->file->next->next->next->next->next->next) {
                current->file->next->next->next->next->next->next = new_file;
            } else if (!current->file->next->next->next->next->next->next->next) {
                current->file->next->next->next->next->next->next->next = new_file;
            } else if (!current->file->next->next->next->next->next->next->next->next) {
                current->file->next->next->next->next->next->next->next->next = new_file;
            } else if (!current->file->next->next->next->next->next->next->next->next->next) {
                current->file->next->next->next->next->next->next->next->next->next = new_file;
            } else if (!current->file->next->next->next->next->next->next->next->next->next->next) {
                current->file->next->next->next->next->next->next->next->next->next->next = new_file;
            } else if (!current->file->next->next->next->next->next->next->next->next->next->next->next) {
                current->file->next->next->next->next->next->next->next->next->next->next->next = new_file;
            } else {
                fprintf(stderr, "Too many files in directory %s.\n", current->name);
            }
            break;
        }
    }
}

// Give me the size of all the child directories you are pointing to
unsigned int get_dir_children_size(DirNode *parent) {
    unsigned int dir_size = 0;

    if (!parent->child) return dir_size;

    DirNode *current = parent->child;
    while (current != parent) {
        File *curr_file = current->file;
        while (curr_file)  {
            dir_size += curr_file->size;
            curr_file = curr_file->next;
        }

        if (current->child) {
            current = current->child;
        } else if (current->next) {
            current = current->next;
        } else {
            while ((current->parent != parent) && !current->parent->next) {
                current = current->parent;
            }
            if (current->parent == parent) break;
            current = current->parent->next;
        }
    }

    return dir_size;
}

// Print the size of all files in the directory
void print_dir_size (void) {
    fprintf(stdout, "\nDIRECTORY SIZES\n");
    unsigned int dir_size;
    unsigned int total_size = 0;

    DirNode *current = &root;
    while (current) {
        dir_size = 0;

        File *curr_file = current->file;
        while (curr_file)  {
            dir_size += curr_file->size;
            curr_file = curr_file->next;
        }
        dir_size += get_dir_children_size(current);
        fprintf(stdout, "Size of dir %s is %u.\n", current->name, dir_size);
        total_size += dir_size;

        if (current->child) {
            current = current->child;
        } else if (current->next) {
            current = current->next;
        } else {
            while (current->parent && !current->parent->next) {
                current = current->parent;
            }
            if (!current->parent) break;
            current = current->parent->next;
        }
    }

    fprintf(stdout, "Total size of dir is %u.\n", total_size);
}

// I know some people are shy about their weight, only print the directories whose size is under max_size
void print_dir_size_under (unsigned int max_size) {
    fprintf(stdout, "\nSUM OF DIRECTORY SIZES UNDER %u\n", max_size);
    unsigned int dir_size;
    unsigned int total_size = 0;

    DirNode *current = &root;
    while (current) {
        dir_size = 0;

        File *curr_file = current->file;
        while (curr_file)  {
            dir_size += curr_file->size;
            curr_file = curr_file->next;
        }
        dir_size += get_dir_children_size(current);
        if (dir_size < max_size) {
            total_size += dir_size;
        }

        if (current->child) {
            current = current->child;
        } else if (current->next) {
            current = current->next;
        } else {
            while (current->parent && !current->parent->next) {
                current = current->parent;
            }
            if (!current->parent) break;
            current = current->parent->next;
        }
    }

    fprintf(stdout, "Sum: %u\n", total_size);
}

// Print the directory with the smallest size after min_size
void print_smallest_dir_size_over (unsigned int min_size) {
    fprintf(stdout, "\nSMALLEST DIRECTORY SIZE OVER %u\n", min_size);
    unsigned int dir_size;
    unsigned int smallest_size = 999999999;

    DirNode *current = &root;
    while (current) {
        dir_size = 0;

        File *curr_file = current->file;
        while (curr_file)  {
            dir_size += curr_file->size;
            curr_file = curr_file->next;
        }
        dir_size += get_dir_children_size(current);
        if (dir_size > min_size) {
            if (dir_size < smallest_size) smallest_size = dir_size;
        }

        if (current->child) {
            current = current->child;
        } else if (current->next) {
            current = current->next;
        } else {
            while (current->parent && !current->parent->next) {
                current = current->parent;
            }
            if (!current->parent) break;
            current = current->parent->next;
        }
    }

    fprintf(stdout, "Smallest size over %u is %u\n", min_size, smallest_size);
}

// It begins here
int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./exe input-file-name\n");
        return 0;
    }

    FILE *ifs = fopen(argv[1], "r");
    if (!ifs) {
        fprintf(stderr, "Invalid file name: %s\n", argv[1]);
        return 0;
    }

    setup_directory_tree(ifs);
    // print_directory_tree();
    print_dir_size();
    // print_dir_size_under(999999999);
    print_smallest_dir_size_over(208860);

    free_directory_tree();

    fclose(ifs);

    return 0;
}