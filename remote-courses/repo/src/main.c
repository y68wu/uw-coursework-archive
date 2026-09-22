// DO NOT TOUCH THIS FILE

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct vector{
    double x;
    double y;
};

// Produces a double value that is the dot product of the two vectors provided
double dot_product(struct vector v1, struct vector v2){
    return (v1.x * v2.x) + (v1.y * v2.y);
}

// Produces the result of multiplying the vector with the given scalar
struct vector scalar_multiply(struct vector v, double scalar){
    struct vector r = {-1,-1};
    r.x = v.x * scalar;
    r.y = v.y * scalar;
    return r;
}

// Produces the magnitude of the vector given
double magnitude(struct vector v){
    return sqrt(dot_product(v,v));
}

// Produces a vector that is the sum of the two vectors provided
struct vector add(struct vector v1, struct vector v2){
    double x_coord = v1.x + v2.x;
    double y_coord = v1.y + v2.y;
    struct vector v = {x_coord, y_coord};
    return v;
}


// Normalizes the vector (ie. produces the unit vector with the same direction)
struct vector normalize(struct vector v){
    struct vector r = {-1,-1};
    // ...
    return r;
}


// Returns a double denoting the angle between the two vectors
double angle_between(struct vector v1, struct vector v2){
    // ...
    return -1;
}

// Returns the vector produced when the vector is projected on the base vector
struct vector project(struct vector v, struct vector base){
    struct vector r = {-1,-1};
    // ...
    return r;
}

// ---------------------- Testing stuff ---------------------------------------
// Prints the vector in a format of (x y) if x and y are its coordinates
void print_vector(struct vector v){
    printf("(%.2f %.2f)\n", v.x , v.y);
}

// This is only for IO testing
// Kind of like a hash-map, but way less efficient
// Assigns a name to each vector created
struct named_vector{
    char *name;
    struct vector v;
};

// This stores all the named vectors (max of 1000)
struct named_vector all_vecs[1000];
int total=0; // keeps track of how many vectors we have so far

// Prints all the vectors named so far
void print_all(){
    for (int i=0; i<total; ++i){
        printf("%s:", all_vecs[i].name);
        print_vector(all_vecs[i].v);
    }
}

// Finds and returns vector with the given name
// If not found, returns {-1,-1}
struct vector find(char *name){
    struct vector v={-1,-1};
    for (int i=0; i<total; ++i){
        if (strcmp(all_vecs[i].name, name)==0){
            return all_vecs[i].v;
        }
    }
    return v;
}

// Puts a new named vector with a given name and value
// If the name already exists, it just adds it to the end of the array
void put_new(char *name, struct vector v){
    for (int i=0; i<total; ++i){
        if (strcmp(all_vecs[i].name, name)==0){
            all_vecs[i].v = v;
            return;
        }
    }
    struct named_vector nv = {strdup(name),v};
    all_vecs[total]=nv;
    ++total;
    return;
}

int main(){
    while (true){

        // Stores the command given
        char cmd[100];
        scanf("%s", cmd);
        
        // ends the loop
        if (strcmp(cmd,"end")==0){
            // to ensure no memory leaks
            for (int i = 0; i < total; ++i) {
                free(all_vecs[i].name);
            }
            break;
        }
        
        // new [vec_name] [x] [y]
        // initalizes vector with vec_name with coords (x y)
        if (strcmp(cmd,"new")==0){
            char name[100];
            struct vector v;
            scanf("%s", name);
            scanf("%lf %lf",&v.x, &v.y);

            put_new(name,v);
        }
        
        // add [vec_f] [vec1] [vec2]
        // stores vec1+vec2 in vec_f
        else if (strcmp(cmd,"add")==0){
            char name[100];
            char v1[100];
            char v2[100];

            struct vector v;

            scanf("%s %s %s",name, v1, v2);
            v = add(find(v1), find(v2));

            put_new(name,v);
        }
        
        // dot_product [vec1] [vec2]
        // prints the output of dot product of vec1 and vec2
        else if (strcmp(cmd,"dot_product")==0){
            char v1[100];
            char v2[100];
            scanf("%s %s",v1,v2);
            printf("Dot product of %s and %s: %.2f\n",v1,v2,dot_product(find(v1),find(v2)));
        }
        
        // scalar_mult [vec_f] [vec] [scalar]
        // stores the result of multiplying scalar by vec into vec_f
        else if (strcmp(cmd,"scalar_mult")==0){
            char name[100]; // name of new vector
            char v[100];    // vector to scale
            double s;       // scalar value

            struct vector r;
            scanf("%s %s %lf", name,v,&s);
            r=scalar_multiply(find(v),s);

            put_new(name,r);
        }
        
        // magnitude [vec]
        // prints the magnitude of vec
        else if (strcmp(cmd,"magnitude")==0){
            char v[100];
            scanf("%s",v);
            // printf("We have: %s",v);
            printf("Magnitude of %s: %.2f\n", v, magnitude(find(v))); 
        }
        
        // normalize [vec_f] [vec]
        // stores the result of normalizing vec into vec_f
        else if (strcmp(cmd,"normalize")==0){
            char name[100]; // name of new vector
            char v[100];    // vector to scale

            struct vector r;
            scanf("%s %s", name,v);
            r=normalize(find(v));

            put_new(name,r);
        }
        
        // angle_bw [vec1] [vec2]
        // prints angle between vec1 and vec2
        else if (strcmp(cmd,"angle_bw")==0){
            char v1[100];
            char v2[100];

            scanf("%s %s", v1,v2);
            double angle = angle_between(find(v1),find(v2));

            printf("Angle between %s and %s: %.2f radians\n", v1,v2,angle);
        }
        
        // project [vec_f] [vec] [base]
        // stores result of project vec onto base into vec_f
        else if (strcmp(cmd,"project")==0){
            char name[100];   //stored projected vector
            char v[100];     //vector TO project
            char base[100];  // base vector (project ON)
            
            struct vector r;
            scanf("%s %s %s",name, v, base);
            r = project(find(v),find(base));

            put_new(name,r);

        }
        print_all(); // print all the vectors at the end of the command
        printf("\n");
    }
    
    return 0;
}
