#Queries for server:
1 - /reg/login/password - reg new acc
2 - /log/login/password - log in acc
3 - /sendto/to_whom/message - send a message to user
4 - /sendreq/to_whom - send a req to user
5 - /getlist/ - get list of friends
6 - /geticreq/ - get list of incoming requests
7 - /getogreq/ - get list of outgoing requests
8 - /reqanswer/to_whom/answer(accept or denied)
    - answer to request
9 - /delfriend/frd_name - del friend
10 - /chstatus/status(smile, angry) - change status
#Answers for client:
1 - /f - something went wrong
2 - /s - something went success
3 - /userlist/usr1/status/usr2/status/.../usrN/status/ - list of friends
4 - /icreqlist/req1/req2/.../reqN/ - list of incoming requests
5 - /ogreqlist/req1/req2/.../reqN/ - list of outgoing requests
6 - /newreq/ - you got new request
7 - /reqanswer/responder/answer:
                        exist - request already exists or user is
                        already your friend;
                        noexist - user isn't exist;
8 - /delfriend/friend_name - frd've deleted you
9 - /log/friend_name/status - informs that friend joined and his status
10 - /chstatus/friend_name/new_status - informs that friend changed his status

