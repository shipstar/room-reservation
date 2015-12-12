import spark from 'spark'

spark.on('login', () => {
  spark.getEventStream(false, 'mine', (data) => {
    console.log("Event: ", JSON.stringify(data))
  })
})

// spark.on('login', function() {
//   spark.getEventStream(false, 'mine', function(data) {
//     console.log("Event: ", JSON.stringify(data))
//   })
// })

spark.login({ accessToken: process.env.SPARK_TOKEN })
