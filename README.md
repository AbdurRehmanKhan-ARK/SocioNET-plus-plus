SocioNET++ 🚀
=============

**SocioNET++** is a high-performance, console-based social media engine. By leveraging **Graph Theory**, it models complex user relationships and interactions, transforming data structures into a functional social ecosystem.

🌟 Core Features
----------------

| Feature | Description |
|--------|------------|
| **User Profiling** | Build rich profiles with metadata, interests, and personalized posts. |
| **Dynamic Graphing** | Directed and undirected edges manage friend requests and confirmed connections. |
| **Social Insights** | Discover mutual friends using **Breadth-First Search (BFS)** logic. |
| **Degrees of Separation** | Calculate the shortest path between any two users in the network. |
| **Data Persistence** | Full state recovery—your social graph is saved and reloaded across sessions. |

🛠 Prerequisites
----------------

Before diving in, ensure you have the following installed:

*   **Compiler:** GCC/G++ (supporting **C++17** or higher)
    
*   **Build Tool:** Make (optional) or G++ CLI
    
*   **IDE:** Visual Studio Code (recommended)
    

🚀 Getting Started
------------------

### 1\. Installation

Clone the repository to your local machine to get started:

```Bash
    git clone https://github.com/syedsufyan-coder/SocioNET-plus-plus.git
    cd SocioNET-plus-plus
```

### 2\. Compilation

This project uses a sources.txt manifest to manage compilation efficiently. To build the executable, run:

```Bash
    g++ -std=c++17 -g @sources.txt -Iinclude -o SocioNET.exe   
```

### 3\. Running the App

Once compiled, launch the system:

```Bash
    ./SocioNET.exe
 ```

📂 Project Architecture
-----------------------

*   **src/**: The engine room. Contains all .cpp logic and graph implementations.
    
*   **include/**: Header files defining the core data structures and classes.
    
*   **docs/**: Deep-dive technical documentation for every module.
    
*   **.vscode/**: Pre-configured tasks.json and launch.json for seamless "One-Click" debugging in VS Code.
    

> \[!NOTE\]
> 
> **Why sources.txt?**
> 
> Instead of manually listing dozens of files in your terminal, sources.txt acts as a build manifest. It ensures that every developer compiles the exact same source set, maintaining integrity across the graph's persistent storage logic.

🌱 Roadmap
----------

We’re just getting started. Future iterations of SocioNET++ will include:

*   **Visual Interface:** Migration from CLI to a GUI using **SFML** or **Qt**.
    
*   **Smart Analytics:** Implementing **PageRank** to identify "Influencer" nodes.
    
*   **Database Integration:** Moving from flat files to **SQL** for enterprise-grade scaling.
    

📚 Technical Reference
----------------------

Need to know the time complexity of our BFS implementation? Check out the /docs folder. It contains detailed Markdown files for every component, covering:

*   Algorithmic complexity ($O(V + E)$).
    
*   Data structure choices.
    
*   Dependency mapping.
    

**Developed with ❤️ using C++ and Graph Theory.**