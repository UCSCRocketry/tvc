# tvc
New TVC repo for Fall 2025

Please follow the below guildlines before making changes

# Guide to Git:
To get started, clone this repo using:
```
git clone https://github.com/username/repo-name.git
cd repo-name
```
In this case, repo-name should be **tvc**

You can copy the url from the green "code" button

### *Make sure you have PULLED CHANGES before pushing your branch!*
#### Update local branch with:
```git pull origin main```

- This will get the latest version from the GitHub repo and merge them into your branch


## BRANCHES (Always make a branch for any changes):

Name your branches with this format:
```<type>/<topic>/<signature>```

Example Types:
- ```feature``` for new functionality
- ```fix``` for fixing a bug in an existing feature
- ```test``` for testing new ideas
- ```refactor``` for changing structure of code, but not actual functionality

```<topic>``` is the specific system/focus of your changes, like:
- ```pid-controller```
- ```avionics```
  
These are less defined than Types, but please try to keep naming consistent.

### Your Signature:

It is important to end your branch name with your signature, both for organization and so you can have credit for what you add.
 - For the format, use ```{first name}{last initial}```
 - Ex: My (Harry Whalen) branch would end with ```/harryw```

If you would strongly prefer to have a different signature, change it to whatever you like but please keep it consistent.

<br>

## Adding your changes:

**Before you begin any work, make the branch for the changes**

- Follow the above naming scheme, then use:
```git checkout -b branch-name```

- You can switch to an existing branch by using:
```git checkout branch-name```

- You can check what branch you are on with:
```git branch```

- Check where your branch is compared to main:
```git status```

Now that you are on your branch, make your changes.

<br>

Once you are *done*, you can **stage** your changes to your branch:
```git add <file-name>```

- using "." instead of "file-name" will commit all files in the working directory

- Now **commit** your changes:

```git commit -m "{describe what you changed within these quotes}"```

This adds a message to your commit, make sure you include one.

<br>


### Now is the time to run ```git pull origin main```
#### This will make sure your branch is up to date with new changes from the remote repo (here in GitHub)

#### If there are conflicts, you must resolve them now before pushing your changes

<br>

#### To push your changes to the remote branch:

```git push origin <your branch name>```

#### Almost done! Now open GitHub, and you should see a prompt to:
```“Compare & pull request”```

<br>

Click this, fill out the prompts, and submit the PR for review.

### Now that your PR has been submitted, *someone else* should review it

### Once it has passed review, it will be merged with the main branch, and you are done :)

- After your branch is merged into main, you can delete it in GitHub by viewing your branches and clicking the trash can
- To delete on your local repo, run
```git branch -d <branch name>```

<br>

# Your first Pull Request:

If this is your first time contributing to this project, make your first change by adding your name to the list of contributors

#### Follow the above instructions, and:

- make your branch ```update-contributors/<signature>```
- edit ```contributors.md``` by adding your name and signature, keeping the same formatting
- Follow the steps to commit and push your changes (don't forget to pull the latest changes first!)
- Review your PR, and merge it with the main branch if there are no conflicts
- (you don't need to wait for someone else to review it this time)
- once you merge it into main, you can delete your branch


