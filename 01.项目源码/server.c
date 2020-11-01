#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mysql.h>  
  
#define SERVER_PORT 8888 
#define LOGI "LOGI"
#define REGI "REGI"
#define SEND "SEND"
#define RECV "RECV"
#define CONN "CONN"
#define QUIT "QUIT"
#define HOST "localhost"
#define USER_NAME "root"
#define PASSWD "123456"
#define DATABASE "mail"
#define PORT 3306

struct st_mail{
    char operation[8];
    char content[952];
    char userid[60];
    char title[64];
    char sendid[60];
};
  
  
struct user_info{
	
	char userid[30];
	char password[30];
	char telnum[30];
};

struct user_check{
	char operation[8];
	char userid[30];
	char password[30];
};

struct recv_mail{
	char userid[30];
	char title[64];
	char content[952];
	char sendid[30];
};

struct st_opt{
    char operation[4];
    char userid[30];
};

int main()
{
	
    int serverSocket;

    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    char buffer[1024];
    int iDataNum;
    
    char msg[] = "This is server!\n";
    char mes[] = "Existed user!\n";
    char mess[] = "Wrong password!!\n";
    char messa[] = "Correct\n";
    
	struct st_mail mail;
    struct user_info user;
    struct user_check userInfo;
    
    char sql[1024];
    int userid;

	MYSQL *conn_ptr;
	MYSQL_RES *res_ptr;
  	MYSQL_ROW sqlrow;
	MYSQL_FIELD *fd;
    int res, i, j;


    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return 1;
    }
  
    int on=1;  
    if((setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }  
  
    bzero(&server_addr, sizeof(server_addr));
 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 
 
    if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
        perror("Connection Failed");
        return 1;
    }

    if(listen(serverSocket, 5) < 0) {
        perror("Listening Failed");
        return 1;
    }
  
  
  
    while(1){
        printf("Listening on port: %d\n", SERVER_PORT);
 
 
        client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
        if(client < 0){
            perror("accept");
            continue;
        }
        if (!fork()) {
            printf("\nReceiving client data...\n");
 
 
            printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));
            printf("Port is %d\n", htons(clientAddr.sin_port));
            memset(buffer, 0, 1024);
            iDataNum = recv(client, buffer, 1024, 0);
            if(iDataNum < 0){
                perror("Receiving Failed\n");
                continue;
            }
            
            
            printf("%d received data is %s\n", iDataNum, buffer);
            
            if(strncmp(buffer, QUIT, 4) == 0){
                close(client);
            }
			else if(strncmp(buffer, SEND, 4) == 0) 
			{
                printf("Client send new mail.\n");
                
				memset(&mail, 0, 2024);
                int length = strlen(buffer);
                for(i = 0; i <=3 ; i++) 
				{
				    mail.operation[i] = buffer[i];
			    }
                
				int flag = 0;
			    j = 0;
			    i++;
			    while(i < length)
				{
				    if(buffer[i] != '\n')
					{
					    if(flag == 0)
						{
						    mail.userid[j] = buffer[i];
					    }
						else if(flag == 1)
						{
						    mail.title[j] = buffer[i];
					    }
						else if(flag == 2)
						{
						    mail.content[j] = buffer[i];
					    }
						else if(flag == 3)
						{
						    mail.sendid[j] = buffer[i];
					    }
						else;
					    j++;
				    }
					else
					{
					    flag++;
					    j = 0;
				    }
				    i++;
			    }
                
                printf("User: %s\n", mail.userid);
                printf("Title: %s\n", mail.title);
                printf("Message: %s\n", mail.content);
		        printf("Sender: %s\n", mail.sendid);
                
                
                
                //QUERY MODULE
                conn_ptr = mysql_init(NULL);  
                if (!conn_ptr) 
				{  
                    printf("mysql_init failed\n");  
                    return EXIT_FAILURE;  
                }
				  
                conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "123456", "mail", 0, NULL, 0);  
                if (conn_ptr) 
				{
					res = mysql_query(conn_ptr, "select title, user_id, content from mail");
					
					if(res)
					{
						printf("SELECT error:%s\n",mysql_error(conn_ptr));
					}
					else 
					{
						res_ptr = mysql_store_result(conn_ptr);
						
						if(res_ptr)
						{
							printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
							j = mysql_num_fields(res_ptr);
							
							if((sqlrow = mysql_fetch_row(res_ptr)))
							{
								printf("ok\n");
							}
							while((sqlrow = mysql_fetch_row(res_ptr)))
							{
								for(i = 0; i < j; i++) 
								{
									printf("%s\t", sqlrow[i]);
								}
								printf("\n");
							}
							
							if (mysql_errno(conn_ptr))
							{
								fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));
							}
						}
						
						mysql_free_result(res_ptr); 
					}
			    }
			    else 
			    {
			    	printf("Connection failed\n");
				}
					
			
				mysql_close(conn_ptr);
				
					
				//INSERT MODULE
				
				conn_ptr = mysql_init(NULL);
				if (!conn_ptr)
				{
					printf("mysql_init failed\n");  
                    return EXIT_FAILURE;  
				}
				
				conn_ptr = mysql_real_connect(conn_ptr, "localhost" , "root", "123456", "mail", 0, NULL, 0); 
				
				if (conn_ptr)
				{
					printf("Database connected \n");
					
					memset(sql, 0, 1024);
					
				//	userid = atoi(mail.userid);
					
					sprintf(sql, "insert into mail (user_id, title, content, sendid) values ('%s', '%s', '%s', '%s');", mail.userid, mail.title, mail.content, mail.sendid);
					
					printf("%s\n", sql);
					
					res = mysql_query(conn_ptr, sql); 
					
					if(!res)
					{
						printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
					}
					
					else
					{
						fprintf(stderr, "Insert error %d: %sn",mysql_errno(conn_ptr),mysql_error(conn_ptr)); 
					}
				}
				
				else 
				{
					printf("Mysql connection failed\n");  
				}
				
				mysql_close(conn_ptr);
		              
                send(client, "The mail was sent successfully !\n", strlen("The mail was sent successfully !!\n"), 0);
                
            }
			else if (strncmp(buffer, RECV, 4) == 0) {//??????

				printf("Client need one specific  mail.\n");

				struct st_opt opt;
				struct recv_mail remail;

				memset(&opt, 0, sizeof(struct st_opt));

				memcpy(&opt, buffer, sizeof(struct st_opt));

				printf("opt:%s", opt.operation);

				printf("%s\n", opt.userid);

				memset(&remail, 0, sizeof(struct recv_mail));

				conn_ptr = mysql_init(NULL);

				if (!conn_ptr) {

					printf("mysql_init failed\n");

					return EXIT_FAILURE;

				}

				conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "123456", "mail", 0, NULL, 0);

				if (conn_ptr) {

					printf("Db connect OK\n");

					memset(sql, 0, 1025);

					sprintf(sql, "select user_id, title, content, sendid from mail where user_id = '%s';", opt.userid);

					printf("%s\n", sql);

					res = mysql_query(conn_ptr, sql);     

					if (!res) {     

						res_ptr = mysql_store_result(conn_ptr);              

						if (res_ptr) {

							int cols = mysql_num_fields(res_ptr);

							printf("%d Cols\n", cols);

							sqlrow = mysql_fetch_row(res_ptr);

							for (i = 0; i < cols; i++)

								printf("%s\t", sqlrow[i]);              //??  

							printf("\n");

							strncpy(remail.content, sqlrow[2], strlen(sqlrow[2]));

							strncpy(remail.title, sqlrow[1], strlen(sqlrow[1]));

							if (mysql_errno(conn_ptr)) {

								fprintf(stderr, "Retrive error:s\n", mysql_error(conn_ptr));

							}

						}

						mysql_free_result(res_ptr);

					}

				}

				else {

					printf("Connection failed\n");

				}

				mysql_close(conn_ptr);

				printf("===========\n");

				printf("%s %s\n",remail.content,remail.title);

				send(client, &remail, sizeof(struct recv_mail), 0);

				//send(client, "Mail send ok!\n", strlen("Mail send ok!\n"), 0);

				printf("get ok!\n");





			}

		

	
            
            //Register Module
			else if(strncmp(buffer, REGI, 4) == 0)
			{
				//query the user_id from the database to judge whether the user had been createds
                conn_ptr = mysql_init(NULL);
                
                if (!conn_ptr)
                {
                	printf("mysql_init failed\n");
                   	return EXIT_FAILURE;
				}
                
                conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "123456", "mail", 0, NULL, 0);
                
                //connect the database successfully
                if (conn_ptr)
                {
                	printf("Database connected\n");
                	//query the database wether the user had been eisted
//                	res = mysql_query(conn_ptr, "select user_id from user");
					
					//get the user_id
//					if(res) 
//					{
//						send(client, mes, strlen(mes), 0);
//					}
                	
                	

					
					//user not existed, insert the user into the database
//					else 
//					{
						
						memset(&user, 0, 1024);
						int length = strlen(buffer);
						
						int flag = 0;
			            j = 0;
		                i = 5;
			
			            while(i < length)
						{
				            if(buffer[i] !='\n')
							{
					            if(flag == 0)
								{
						            user.userid[j] = buffer[i];
					            }
								else if(flag == 1)
								{
                    				user.password[j] = buffer[i];
					            }
								else if(flag == 2)
								{
						            user.telnum[j]=buffer[i];
					            }
								else;
					            j++;
				            }
							else
							{
					            flag++;
					            j = 0;
				            }
				            i++;
		            	}
		            	
		            	printf("Userid: %s\n",user.userid);
                        printf("password: %s\n", user.password);
                        printf("telphone: %s\n", user.telnum);
                        
                        //insert the information into the database table
                       	
                    	memset(sql, 0, 1024);
//                    	sprintf(sql, "insert into mail (user_id, title, content, sendid) values ('%s', '%s', '%s', '%s');", mail.userid, mail.title, mail.content, mail.sendid);
						sprintf(sql, "insert into user (user_id, password, phone_num) values ('%s', '%s', '%s');", user.userid, user.password, user.telnum);
						printf("Insert successfully!\n");
						printf("%s\n", sql);
						
//					}
				}
                
                //fail to connect the database
                else 
				{  
                    printf("Mysql connection failed\n");  
                }  
            }
            
            //Login Module
            //judge whether the user's password correct
            else if(strncmp(buffer, LOGI, 4) == 0)
            {
            	//receive the userid & password
            	printf("\nChecking the password...\n");
            	memset(&userInfo, 0, 1024);
            	  int length = strlen(buffer);
                memset(&user, 0, 1024);
						
						int flag = 0;
			            j = 0;
		                i = 5;
			
			            while(i < length)
						{
				            if(buffer[i] !='\n')
							{
					            if(flag == 0)
								{
						            userInfo.userid[j] = buffer[i];
					            }
								else if(flag == 1)
								{
                    				userInfo.password[j] = buffer[i];
					            }
								
								else;
					            j++;
				            }
							else
							{
					            flag++;
					            j = 0;
				            }
				            i++;
		            	}
            	printf("User Info obtained:\n");
            	printf("Userid: %s\n", userInfo.userid);
            	printf("Userpassword: %s\n", userInfo.password);
            	
            	conn_ptr = mysql_init(NULL);  
			    if (!conn_ptr) 
				{  
			        return EXIT_FAILURE;  
			    }  
			    conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "123456", "mail", 0, NULL, 0);
			    
			    if (conn_ptr)
			    {	
					memset(sql, 0, sizeof(sql));
					sprintf(sql,"select user_id,password from user where user_id='%s' and password='%s';", userInfo.userid, userInfo.password);
			    	res = mysql_query(conn_ptr,sql);
			    	
					//not match
					if (res) 
					{         
         		
         				printf("SELECT error:%s\n",mysql_error(conn_ptr));
        			}
        			//
        			else 
        			{
        				res_ptr=mysql_store_result(conn_ptr);
        				if(res_ptr){
        					if((unsigned long)mysql_num_rows(res_ptr)==1){
        						printf("Corrent!\n");
        						send(client, messa, strlen(messa), 0);
							}else{
								printf("not Corrent!\n");
								send(client, mess, strlen(mess), 0);
							}
						}
						mysql_free_result(res_ptr);     				
					}
				
				}
			}
        }
    }
    return 0;
}
