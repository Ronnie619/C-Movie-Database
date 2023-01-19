#include <bits/stdc++.h>
using namespace std;

struct node
{
    int year;
    string title;
    list<string> actors;
    node *left;
    node *right;
};


node *createnode(int year, string title, list<string> actors) //utility function
{
    node *n = new node;
    n->year = year;
    n->title = title;
    n->actors = actors;
    n->left = NULL;
    n->right = NULL;
}

class MovieDB
{
public:
    node *root = NULL;

    //methods of class
    MovieDB();
    node *filltree(node *root, int year, string title, list<string> actors);
    node *add_movie(node *root);
    node *delete_by_title(node *root, string title);
    void print_titles(node *root);
    void print_movies_year(node *p, int key);
    void print_actors_movies(node *root, string name);
    void print_movies_actors(node *root, string title);
};

MovieDB::MovieDB()
{
    // file open here
    ifstream inf;
    inf.open("movies.txt");
    {
        if (!inf.is_open())
        {
            cout << "\nerror\n";
        }
        string x;
        while (getline(inf, x))
        {
            if (x == "")
            {
                continue;
            }
            int index = x.find_last_of(" ");
            string title = x.substr(0, index);
            string year = x.substr(index + 2, x.length() - index - 3);
            list<string> actor;
            while (getline(inf, x))
            {
                if (x == "")
                {
                    break;
                }
                actor.push_back(x);
            }
            root = filltree(root, atoi(year.c_str()), title, actor);
        }
        inf.close();
    }
}

node *MovieDB::filltree(node *root, int year, string title, list<string> actors)
{
    if (root == NULL)
    {
        node *n = createnode(year, title, actors);
        root = n;
        return root;
    }
    else if (year < root->year)
    {
        root->left = filltree(root->left, year, title, actors);
    }
    else
    {
        root->right = filltree(root->right, year, title, actors);
    }
    return root;
}

// pre-order approach
void MovieDB::print_titles(node *root)
{
    if (root != NULL)
    {
        cout << root->title << endl;
        print_titles(root->left);
        print_titles(root->right);
    }
}

// in-order approach
void MovieDB::print_movies_year(node *root, int key)
{
    if (root != NULL)
    {
        print_movies_year(root->left, key);
        if (root->year == key)
        {
            cout << root->title << " (" << root->year << ")" << endl;
            for (auto it = root->actors.begin(); it != root->actors.end(); it++)
            {
                cout << *it << endl;
            }
        }
        print_movies_year(root->right, key);
    }
}

// post-order
void MovieDB::print_actors_movies(node *root, string name)
{
    if (root != NULL)
    {
        print_actors_movies(root->left, name);
        print_actors_movies(root->right, name);

        for (auto it = root->actors.begin(); it != root->actors.end(); it++)
        {
            if (name == *it)
            {
                cout << root->title << " (" << root->year << ")" << endl;
                // it=root->actors.end()-1;
                break;
            }
        }
    }
}

void MovieDB::print_movies_actors(node *root, string title)
{
    if (root == NULL)
    {
        return;
    }
    if (root != NULL)
    {
        print_movies_actors(root->left, title);

        if (title == root->title)
        {
            cout << root->title << " (" << root->year << ")" << endl;
            for (auto it = root->actors.begin(); it != root->actors.end(); it++)
            {
                cout << *it << endl;
            }
            return;
        }
        print_movies_actors(root->right, title);
    }
}

// add movies
node *MovieDB::add_movie(node *root)
{

    string title;
    cout << "Please enter the title of the film: " << endl;
    cin.ignore();
    getline(cin, title); // Gets title from user, assigns title to title
    cout << endl;
    int Year;
    cout << "Please enter Year of the film: " << endl;
    cin >> Year;

    cout << "Please enter names of Actors in the film :" << endl;
    char ch = '1';
    string name;
    list<string> actor;
    while (ch == '1')
    {
        cin.ignore();
        getline(cin, name);
        actor.push_back(name);
        cout << "press 1 to add another name or any key to exit :";
        cin >> ch;
    }

    // added in current BST
    root = filltree(root, Year, title, actor);

    // adding in backend database
    ofstream outf("movies.txt", ios::app);
    {
        if (!outf.is_open())
        {
            cout << "Error\n";
        }
        outf << endl
             << title << " (" << Year << ")" << endl;
        for (auto it = actor.begin(); it != actor.end(); it++)
        {
            outf << *it << endl;
        }
        outf.close();
    }
    cout << "\nMovie added to database Successfully!" << endl;
}

// delete movie
node *traversal(node *root, string title)
{
    static node *x = NULL;
    if (root == NULL)
    {
        return NULL;
    }

    else
    {
        traversal(root->left, title);
        if (root->title == title)
        {
            x = root;
        }
        traversal(root->right, title);
    }
    return x;
}

node *preorder(node *root)
{

    while (root->right != NULL)
    {
        root = root->right;
    }
    return root;
}

node *postorder(node *root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

node *MovieDB::delete_by_title(node *root, string title)
{
    node *p = traversal(root, title);
    if (p == NULL)
    {
        cout << "movie doesn't exist\n";
        return NULL;
    }
    node *prev = root;
    if (p->title == prev->title)
    {
        if (prev->left == NULL && prev->right == NULL)
        {
            node *n = prev;
            prev = NULL;
            delete n;
            return prev;
        }
        else if (prev->left != NULL)
        {
            node *n = preorder(prev->left);
            prev->title = n->title;
            prev->year = n->year;
            prev->actors = n->actors;
            prev->left = delete_by_title(prev->left, n->title);
        }
        else if (prev->right != NULL)
        {
            node *n = postorder(prev->right);
            prev->title = n->title;
            prev->year = n->year;
            prev->actors = n->actors;
            prev->right = delete_by_title(prev->right, n->title);
        }
    }
    else
    {
        while (prev->right->title != p->title && prev->left->title != p->title && prev->right->year != p->year && prev->left->year != p->year)
        {
            if (p->year < prev->year)
            {
                prev = prev->left;
            }
            else
            {
                prev = prev->right;
            }
        }
        if (p->year < prev->year)
        {
            prev->left = delete_by_title(p, title);
        }
        else
        {
            prev->right = delete_by_title(p, title);
        }
    }
    return prev;
}

// main menu
int main()
{
    int choice = 0;
    int quit = 0;
    MovieDB *obj = new MovieDB; //initialized dynemically
    
    do // User interface loop
    {
        cout << "\nWelcome to the Movie Store. \nWould you like to:";
        cout << "\n(1) See what movies are available? \n(2) Search for an actor?";
        cout << "\n(3) Search for a year? \n(4) Search for a movie?";
        cout << "\n(5) Add a movie? \n(6) Delete a movie?";
        cout << "\n(0) Exit the Store" << endl;
        cin >> choice;

        switch (choice)
        {

        case 0:
            cout << "Thank you come again." << endl;
            quit = 1;
            break;
        case 1: // Call to print movie titles
            //   print_titles (root);
            obj->print_titles(obj->root);
            break;
        case 2:
        {
            string name;
            cout << "Please enter actor's name: " << endl;
            cin.ignore();
            getline(cin, name); // Gets name form user, assigns name to name
            cout << endl;
            cout << "Movies with " << name << ":" << endl;
            obj->print_actors_movies(obj->root, name); // Call to print an actor's films
            break;
        }

        case 3:
            int year;
            cout << "Please enter the year you wish to search for: " << endl;
            cin >> year;
            cout << endl;
            cout << "Films made in " << year << ":" << endl;
            obj->print_movies_year(obj->root, year); // Call to print a year's films
            break;

        case 4:
        {
            string title;
            cout << "Please enter the title of the film: " << endl;
            cin.ignore();
            getline(cin, title); // Gets title from user, assigns title to title
            cout << endl;
            cout << "Actors in the film " << title << ":" << endl;
            obj->print_movies_actors(obj->root, title); // Call to print a films's actors
            break;
        }
        case 5:
        {
            obj->add_movie(obj->root); // Call to Add a new film
            break;
        }
        case 6:
        {
            string title;
            cout << "Please enter the title of the film: " << endl;
            cin.ignore();
            getline(cin, title); // Gets title from user, assigns title to title
            cout << endl;
            obj->delete_by_title(obj->root, title); // Call to Add a new film
            break;
        }
        default: // Default case
            cout << "Try again." << endl;
        }
        system("pause");
        system("CLS");
    } while (quit == 0);

    return 0;
}