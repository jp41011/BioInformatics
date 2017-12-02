#comment in R

#load file into var x
#x <- read.table("mappability-output-200", header=TRUE, sep="")

#plot(x[,3], x[,4], abline(a=0, b=1))


#plot with labels
#plot(x[,3]
#	, x[,4]
#	, abline(a=0, b=1)
#	, main="Scatter Plot Between 50 & 70"
#	,xlab="len=50", ylab="len=70")

#make histogram
#hist(x[,3], breaks=seq(0,100,1))

#hist(x[,3], breaks=seq(0,100,5))

#plot(density(x[,3]), ylim=c(0,1))

#plot(density(x[,3]))


#hist(x[,3], breaks=seq(0,100,1), prob=T)
#lines(density(x[,3]), col=2)

hist(x[,3], breaks=seq(0,100,1), prob=T)
lines(density(x[,3]), col=2, ylim=c(0,1))

