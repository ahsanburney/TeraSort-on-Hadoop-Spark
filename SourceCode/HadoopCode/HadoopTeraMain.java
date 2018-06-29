

import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class TeraMain {

	public static void main(String[] args) throws Exception {
		
		if (nArgs.length == 2) {
		} else {
	    	System.err.println("Please enter input and output file");
	    	System.exit(2);
	    }

		String[] argData;	
		argData = new GenericOptionsParser(conf, args).getRemainingArgs();
		Configuration conf = new Configuration();	
	    
	    
	    Job job = new Job(conf, "Terasort");	
	    job.setCombinerClass(TeraHadoopReducer.class);
	    job.setJarByClass(TeraMain.class);	
	    job.setMapperClass(TeraHadoopMapper.class);
	    job.setOutputKeyClass(OutputTxt.class);
	    job.setReducerClass(TeraHadoopReducer.class);
	    job.setOutputValueClass(OutputTxt.class);
	    FileOutputFormat.setOutputPath(job, new Path(argData[1]));
	    FileInputFormat.addInputPath(job, new Path(argData[0])); 
	    System.exit(job.waitForCompletion(true) ? 0 : 1);

	}

}