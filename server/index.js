import dotenv from 'dotenv'
dotenv.load()

import spark from 'spark'
import { startEvent, endEvent } from './gcal'
import dataStore from './dataStore'

const DEFAULT_DURATION = 15 // minutes

spark.on('login', () => {
  spark.getEventStream(false, 'mine', (event) => {
    console.log("Event: ", JSON.stringify(event))

    switch (event.name) {
      case 'spark/status':
        if (event.data === 'online') {
          dataStore[event.coreid] = { coreOnline: true }
        } else {
          // end active events for this room
          // dataStore[event.coreid]
        }
        break;

      case 'room_occupied':
        const googleCalendarId = dataStore[event.coreid].googleCalendarId
        if (event.data === 'OCCUPIED') {

          // console.log(event.coreid, googleCalendarId)
          startEvent(googleCalendarId, { duration: DEFAULT_DURATION },
              event.coreid)
          .then((eventId) => {
            dataStore[event.coreid] = {
              occupied: true
            }
          })
        } else if (event.data === 'NOT_OCCUPIED') {
          // const { googleCalendarId, eventId } = dataStore[event.coreid]

          endEvent(googleCalendarId, { event: dataStore[event.coreid] }, event.coreid)
          .then(() => {
            dataStore[event.coreid] = {
              occupied: false,
              eventId: null,
              eventRsp: null
            }
          })
        }
        break

      default:
        console.log(`Unhandled event type: ${event.name}`)
        break
    }
  })
})

spark.login({ accessToken: process.env.SPARK_TOKEN })
