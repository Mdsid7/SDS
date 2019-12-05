#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
int row,col,f=0,k=0,q=0;
char copy[200]="echo 'sudo ";
char *distro[]={
		"Ubuntu",//apt
		"Linux Mint",//apt
		"Parrot",//apt
		"Debian",//apt
		"MX Linux",//apt
		"Archlinux",//pacman
		"Manjaro",//pacman
		"Void Linux",//xbps
		"OpenSUSE",//yast
		"CentOS",//yum
};
char *apps[]={
              "chromium ",
              "firefox ",
              "vim ",
              "emacs ",
              "java ",
              "rust ",
              "python ",
              "ffmpeg ",
              "openssh ",
              "libreoffice ",
              "thunderbird ",
              "vlc ",
              "rsync ",
	      "ranger ",
};

int c;
int n_distro = sizeof(distro) / sizeof(char *);
int n_apps = sizeof(apps) / sizeof(char *);
int a[100];
void print_distro(WINDOW *temp, int highlight,char *trav[],int n_choices)
{
  int x, y, i;	

  x = 2;
  y = 3;
  box(temp, 0, 0);
  for(i = 0; i < n_choices; ++i)
    {	if(highlight == i + 1) /* High light the present choice */
	{
	  wattron(temp, A_REVERSE); 
	  mvwprintw(temp, y, x, "%s", trav[i]);
	  wattroff(temp, A_REVERSE);
	}
      else
	mvwprintw(temp, y, x, "%s", trav[i]);
      ++y;
    }
  wrefresh(temp);
}
void print_apps(WINDOW *temp,int highlight,int choice)
{
  int x, y, i;	
  x = 2;
  y = 3;
  box(temp, 0, 0);
  for(i = 0; i < n_apps; ++i)
    {
      if(highlight == i + 1) /* High light the present choice */
	{	wattron(temp, A_REVERSE); 
	  mvwprintw(temp, y, x, "%s", apps[i]);
	  wattroff(temp, A_REVERSE);
	}
      else
	mvwprintw(temp, y, x, "%s", apps[i]);
      ++y;
    }
  wrefresh(temp);
}
void hld(WINDOW *temp,WINDOW *trap,int n_choices,char *trav[]){
  keypad(temp, TRUE);
  int choice=0;
  refresh();
  int highlight=1;
  mvwprintw(temp,1,2,"Apps to install:");
  mvwprintw(temp,row-2,2,"Press backspace when selection is done");
  print_apps(temp, highlight,choice);
  while(1)
    {	c = wgetch(temp);
      switch(c)
	{	case KEY_UP:
	    if(highlight == 1)
	      highlight = n_choices;
	    else
	      --highlight;
	    break;
	case KEY_DOWN:
	  if(highlight == n_choices)
	    highlight = 1;
	  else 
	    ++highlight;
	  break;
	case KEY_BACKSPACE:
	  q=1;
	  break;
	case 10:
	  choice = highlight;
	  break;
	default:
	  continue;
	}
      print_apps(temp, highlight,choice);
      if(q==1){
	break;
      }
      if (choice==highlight){
	a[k++]=choice;
	strcat(copy,apps[choice-1]);
	mvwprintw(trap,++f+1,1,"%s",apps[choice-1]);
	wrefresh(trap);
      }
      /* User did a choice come out of the infinite loop */
 	       
    }	
  
  clrtoeol();
  refresh(); 
  endwin(); 
}

void hl(WINDOW *temp,int n_choices,char *trav[]){
  keypad(temp, TRUE);
  int choice=0;
  refresh();
  int highlight=1;
  mvwprintw(temp,1,2,"Select your linux distribution:");
  print_distro(temp, highlight,trav,n_choices);
  while(1)
    {	c = wgetch(temp);
      switch(c)
	{
	case KEY_UP:
	    if(highlight == 1)
	      highlight = n_choices;
	    else
	      --highlight;
	    break;
	case KEY_DOWN:
	  if(highlight == n_choices)
	    highlight = 1;
	  else 
	    ++highlight;
	  break;
	case 10:
	  choice = highlight;
	  break;
	default:
	  continue;
	}
      print_distro(temp, highlight,trav,n_choices);
      if (choice==highlight){
	if (choice<=5){
	  strcat(copy,"apt upgrade && sudo apt install ");
	}
	if (choice==6||choice==7){
	  strcat(copy,"pacman -Syu ");
	}
	if (choice==8){
	  strcat(copy,"xbps-install ");
	}
	if (choice==9){
	  strcat(copy,"zypper in ");
	}
	if (choice==10){
	  strcat(copy,"yum install ");
	}
      }
      if(choice != 0)	/* User did a choice come out of the infinite loop */
	break;
    }	
	
  clrtoeol();
  refresh();
  endwin();
}
void main(int argc,char *argv[])
{
  WINDOW *dist,*app,*lists;
  
  system("clear");
  initscr();
  keypad(stdscr, TRUE);
  cbreak();
  noecho();
  refresh();
  curs_set(0);
  getmaxyx(stdscr,row,col);
  dist=newwin(row,col,0,0);
  box(dist,0,0);
  hl(dist,n_distro,distro);
  lists=newwin(row,col/2,0,col/2);
  box(lists,0,0);
  mvwprintw(lists,1,1,"Selected apps are:");
  wrefresh(lists);
  app=newwin(row,col/2,0,0);
  box(app,0,0);
  hld(app,lists,n_apps,apps);
  strcat(copy,"' >> deploy.sh");
  system(copy);
  printf("%s\n",copy);
  printf("Do you want to run the command?\n");
  printf("1.YES\n");
  printf("2.NO\n");
  scanf("%d",&f);
  if (f==1){
    system("chmod +x deploy.sh && ./deploy.sh");
  }
  else
    printf("Exiting...");
  system("rm deploy.sh");
}
