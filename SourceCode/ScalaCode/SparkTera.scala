val Filelines = sc.textFile("/root/input.txt")
val StartTime = System.nanoTime
val sort = Filelines.map(_.split("  ")).map(arr => (arr(0) + "  " + arr(1),arr.mkString("  "))).sortByKey().map(_._2)
val FileSize = Filelines*100;
println("file Size(bytes) :")
print(FileSize)
val SplitTime = (System.nanoTime - t1) / 1e9d
println("Sorting time for splitted file  :")
print(SplitTime)
sort.saveAsTextFile("/root/output.txt")
val TotalTeraSortTime = (System.nanoTime - t1) / 1e9d
println("Total Spark  Tera Sort Time (Sort, Merge and Transfer To Local):")
print(TotalTeraSortTime)
