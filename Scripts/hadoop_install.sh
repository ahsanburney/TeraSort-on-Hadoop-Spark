#!/bin/sh
echo " Install JDK and JRE "
sudo apt-get update
sudo apt-get install default-jre
sudo apt-get install default-jdk
sudo apt-get install vim
sudo apt-get install gdb
sudo apt-get install gcc
sudo apt-get install g++

wget -c --header "Cookie: oraclelicense=accept-securebackup-cookie" http://download.oracle.com/otn-pub/java/jdk/8u131-b11/d54c1d3a095b4ff2b6607d096fa80163/jdk-8u131-linux-x64.tar.gz
tar -xvzf jdk-8u131-linux-x64.tar.gz
ln -s jdk1.8.0_131 jdk


echo "Download Hadoop"
wget https://dist.apache.org/repos/dist/release/hadoop/common/hadoop-2.8.1/hadoop-2.8.1.tar.gz
tar xfz hadoop-2.8.1.tar.gz
ln -s hadoop-2.8.1 hadoop

echo "ssh localhost"
ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

echo "Path Set"
echo "export JAVA_HOME=/home/ubuntu/jdk
export HADOOP_INSTALL=/home/ubuntu/hadoop
export PATH=\$PATH:\$HADOOP_INSTALL/bin
export PATH=\$PATH:\$HADOOP_INSTALL/sbin
export HADOOP_MAPRED_HOME=\$HADOOP_INSTALL
export HADOOP_COMMON_HOME=\$HADOOP_INSTALL
export HADOOP_HDFS_HOME=\$HADOOP_INSTALL
export YARN_HOME=\$HADOOP_INSTALL" >> ~/.bashrc
~
export JAVA_HOME=/home/ubuntu/jdk
export HADOOP_INSTALL=/home/ubuntu/hadoop
export PATH=$PATH:$HADOOP_INSTALL/bin
export PATH=$PATH:$HADOOP_INSTALL/sbin
export HADOOP_MAPRED_HOME=$HADOOP_INSTALL
export HADOOP_COMMON_HOME=$HADOOP_INSTALL
export HADOOP_HDFS_HOME=$HADOOP_INSTALL
export YARN_HOME=$HADOOP_INSTALL


echo "Making Setting in Hadoop for single Node"
echo $(wget -qO- http://instance-data/latest/meta-data/public-ipv4) > hadoop/etc/hadoop/masters
echo $(wget -qO- http://instance-data/latest/meta-data/public-ipv4) > hadoop/etc/hadoop/slaves

echo "<property>
    <name>hadoop.tmp.dir</name>
    <value>/home/ubuntu/tmp</value>
  </property>
  <property>
    <name>fs.default.name</name>
    <value>hdfs://$(wget -qO- http://instance-data/latest/meta-data/local-ipv4)</value>
  </property>" > sam3
sed  '/<configuration>/r sam3' hadoop/etc/hadoop/core-site.xml > sam4
mv sam4 hadoop/etc/hadoop/core-site.xml

 #!/bin/sh
echo " <property>
  <name>dfs.replication</name>
    <value>1</value>
  </property>" > sam3
sed  '/<configuration>/r sam3' hadoop/etc/hadoop/hdfs-site.xml > sam4
mv sam4  hadoop/etc/hadoop/hdfs-site.xml

cp hadoop/etc/hadoop/mapred-site.xml.template hadoop/etc/hadoop/mapred-site.xml
echo " <property>
    <name>mapred.job.tracker</name>
    <value>$(wget -qO- http://instance-data/latest/meta-data/public-ipv4):54311</value>
  </property>" > sam3
sed  '/<configuration>/r sam3' hadoop/etc/hadoop/mapred-site.xml > sam4
mv sam4 hadoop/etc/hadoop/mapred-site.xml

echo " <property>
    <name>yarn.nodemanager.aux-services</name>
    <value>mapreduce_shuffle</value>
  </property>
  <property>
    <name>yarn.resourcemanager.address</name>
    <value>localhost:50040</value>
  </property>
  <property>
    <name>yarn.nodemanager.address</name>
    <value>localhost:50050</value>
  </property>
  <property>
    <name>yarn.nodemanager.localizer.address</name>
    <value>localhost:50060</value>
  </property>" > sam3
sed  '/<configuration>/r sam3' hadoop/etc/hadoop/yarn-site.xml > sam4
mv sam4 hadoop/etc/hadoop/yarn-site.xml
rm sam3
sed -i 's/${JAVA_HOME}/\/home\/ubuntu\/jdk/g' hadoop/etc/hadoop/hadoop-env.sh

echo "Start HDFS DFS YARN"
hdfs namenode -format
start-dfs.sh
start-yarn.sh
echo "Generating 128GB File"
./gensort -a 1374389535 input.txt

echo "Transfer input.txt to Hadoop server"
hdfs dfs -copyFromLocal input.txt /input

echo "Executing Hadoop TeraSort"
hadoop jar HadoopTeraSort.jar /input /output

echo "Transfer outputfile from Hadoop server to local"
hdfs dfs -copyToLocal /output/part* ./
