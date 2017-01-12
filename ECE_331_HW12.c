/* Joseph Garcia */
/* ECE 331 Sheaff */
/* Homework 12 */
/* 4/22/15 */
1.)
m   h    dom mon dow  user    command
5  11     */2 *   *   root    xkcd
55 23     */2 *   *   root    xkcd 

2. Paper attached

3.)
a. Scott Edgerly and Mao Yianxiang
b. 
	$ sudo aptitude install nfs-common nfs-kernel-server rpc-bind portmap
	$ sudo mkdir /home/client1  
	$ sudo chown nobody:nogroup /home/client1
	$ sudo chmod 755 /home/client1
	
	$ sudo vim /etc/exports
	#added this line
	/home/client1	192.168.0.*(rw,sync,no_subtree_check)

	$ sudo /etc/init.d/nfs-kernel-server restart
	$ sudo /etc/init.d/rpcbind restart
	
c. /home/client1
d. Modified /etc/exports on the server side, added the line /home/client1 192.168.0.*(rw,sync,no_subtree_check)
e. sudo mount 192.168.0.25:/home/client1/ /mnt/nfs_import
f. sudo mkdir /mnt/nfs_import 





