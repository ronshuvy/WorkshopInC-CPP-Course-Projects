
// functions 

    [[nodiscard]]  map<const string, vector<double>> getMovies() const { return _movies; }
    [[nodiscard]]  map<const string, vector<double>> getUsers() const { return _users; }
    [[nodiscard]] vector<string> getTitles() const { return _titlesInOrder; }


void printList(const vector<double>& MyList)
{
    cout << "LIST -" << endl;
    for (const auto & elem : MyList)
    {
        cout << elem << " ";
    }
}
