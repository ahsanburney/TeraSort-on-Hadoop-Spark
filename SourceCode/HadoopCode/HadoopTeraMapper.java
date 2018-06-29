import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Mapper.Context;

public class TeraMapper extends Mapper<Object, Text, Text, Text> {
	private Text nKey;
    private Text nValue;
	
	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		String word = value.toString();
		String[] token = word.split("\\n");
		 
		for(String tkn: token){
			nKey = new Text(tkn.substring(0, 10));
			nValue = new Text(tkn.substring(10, tkn.length()));
			context.write(nKey, nValue);
		}
	}
}