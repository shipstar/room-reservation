import dotenv from 'dotenv'
dotenv.load()

import spark from 'spark'
import { startEvent, endEvent } from './gcal'
import dataStore from './dataStore'

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
        if (event.data === 'OCCUPIED') {
          const { googleCalendarId } = dataStore[event.coreid]

          startEvent(googleCalendarId, { duration: DEFAULT_DURATION })
          .then((eventId) => {
            dataStore[event.coreid] = {
              occupied: true,
              eventId
            }

            console.warn(eventId)
          })
        } else if (event.data === 'NOT OCCUPIED') {
          const { googleCalendarId, eventId } = dataStore[event.coreid]

          endEvent(googleCalendarId, { eventId })
          .then(() => {
            dataStore[event.coreid] = {
              occupied: false,
              eventId: null
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
