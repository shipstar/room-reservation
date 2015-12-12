var spark = require('spark')

spark.on('login', function() {
  spark.getEventStream(false, 'mine', function(data) {
    console.log("Event: ", JSON.stringify(data))
  })
})

spark.login({ accessToken: process.env.SPARK_TOKEN })
