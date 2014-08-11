#include <istream>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

typedef struct treeNode //Tree Node Declaration
{
  int year;
  string title;
  list < string > actors;
  treeNode *left;
  treeNode *right;

} *treePtr;

treePtr root = NULL;

//Function Prototypes
treePtr fillTree (treePtr p, int y, string t, list < string > a);
void print_titles (treePtr root);
void print_movies_year (treePtr p, int key);
void print_actors_movies (treePtr root, string name);
void print_movies_actors (treePtr root, string title);

int
main ()
{

  ifstream inFile ("movies.txt");
  string x;
  treePtr root = NULL;
  int count = 0;
  if (inFile.is_open ()) //Check if file opened sucessfully
    {

      while (getline (inFile, x)) //Get title
	{
	  if (x == "") //Check for space
	    {
	      continue;
	    }
	  count++;

	  int index = x.find_last_of (" "); //Code to eliminate () from year
	  string title = x.substr (0, index);
	  string year = x.substr (index + 2, x.length () - index - 3);

	  list < string > actor; //Linked list declaration
	  int counter = 0;
	  while (getline (inFile, x)) //loop to read actors into list
	    {
	      if (x == "")
		{
		  break;
		}
	      else
		{
		  actor.push_back (x);
		}
	    }

      root = fillTree (root, atoi (year.c_str ()), title, actor); //Fills BST
	}
    }

  int choice;
  int quit = 0;

  do				//User interface loop
    {
      cout << 
	"\nWelcome to the Movie Store. \nWould you like to:";
      cout <<
	"\n(1) See what movies are available? \n(2) Search for an actor?";
      cout << 
	"\n(3) Search for a year? \n(4) Search for a movie?";
      cout << 
	"\n(0) Exit the Store" << endl;
      cin >> choice;

      switch (choice)
	{

	case 0:
	  cout << "Thank you come again." << endl;
	  quit = 1;
	  break;
	case 1: //Call to print movie titles
	  print_titles (root);
	  break;  
	case 2:
	  {
	    string name;
	    cout << "Please enter actor's name: " << endl;
	    cin.ignore ();
	    getline (cin, name); //Gets name form user, assigns name to name
	    cout << endl;
	    cout << "Movies with " << name << ":" << endl;
	    print_actors_movies (root, name); //Call to print an actor's films
	    break;
	  }

	case 3:
	  int year;
	  cout << "Please enter the year you wish to search for: " << endl;
	  cin >> year;
	  cout << endl;
	  cout << "Films made in " << year << ":" << endl;
	  print_movies_year (root, year); //Call to print a year's films
	  break;

	case 4:
	  {
	    string title;
	    cout << "Please enter the title of the film: " << endl;
	    cin.ignore ();
	    getline (cin, title);//Gets title from user, assigns title to title
	    cout << endl;
	    cout << "Actors in the film " << title << ":" << endl;
	    print_movies_actors (root, title);//Call to print a films's actors
	    break;
	  }

	default: //Default case
	  cout << "Try again." << endl;

	}

    }
  while (quit == 0);

  return 0;
}

//Program Functions

treePtr
fillTree (treePtr p, int y, string t, list < string > a)//Fill Tree
{
  treePtr n = new treeNode;
  n->year = y;
  n->title = t;
  n->actors = a;
  n->left = NULL;
  n->right = NULL;
  if (p == NULL) //First node
    {
      p = n;
    }
  else
    {
      treePtr prev = p;
      treePtr curr = p;
      while (curr != NULL) //Organize tree using year as key
	{
	  if (curr->year > y)
	    {
	      prev = curr;
	      curr = curr->left;
	    }
	  else
	    {
	      prev = curr;
	      curr = curr->right;
	    }
	}
      if (prev->year > y)
	{
	  prev->left = n;
	}
      else
	{
	  prev->right = n;
	}
    }

  return p;
}

void
print_titles (treePtr root) //Prints film titles
{
  if (root == NULL)
    return;
  if (root->left) //Iterate through tree printing in order
    print_titles (root->left);
  cout << root->title << endl;
  if (root->right)
    print_titles (root->right);
}

void
print_movies_year (treePtr p, int key) //Prints films for a given year
{

  if (p == NULL)
    return;
  if (p->left) //Iterates through tree printing in order
    print_movies_year (p->left, key);
  if (p->year == key)
    {
      cout << p->title << endl;
    }
  if (p->right)
    print_movies_year (p->right, key);
}

void
print_actors_movies (treePtr root, string name)//Prints movies of given actor
{
  if (root == NULL)
    return;
  if (root->left)
    print_actors_movies (root->left, name);
	//loop to iterate through linked list to locate actor
  for (list < string >::iterator it = root->actors.begin ();
       it != root->actors.end (); ++it)
    {

      if (*it == name) //Prints title if actor is found in list
	{
	  cout << root->title << endl;
	}
    }
  if (root->right)
    print_actors_movies (root->right, name);

}


void //Prints actors of a given film
print_movies_actors (treePtr root, string title)
{
  if (root == NULL)
    return;
  if (root->left)
    print_movies_actors (root->left, title);

  if (root->title == title)
    { //Loop to print actors if title is found in tree node
      for (list < string >::iterator it = root->actors.begin (); 
			it != root->actors.end (); ++it)	
{
	  cout << *it << endl;

	}
    }

  if (root->right)
    print_movies_actors (root->right, title);
}
