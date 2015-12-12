import spark from 'spark'
import { startEvent, endEvent } from './gcal'

const DEFAULT_DURATION = 15 // minutes

let rooms = {
  'abc': {
    name: 'Conference Room ABC',
    coreOnline: false,
    occupied: false,
    eventId: null,
    googleCalendarId: 'primary'
  },
  'xyz': {
    name: 'Breakout XYZ',
    coreOnline: false,
    occupied: false,
    eventId: null,
    googleCalendarId: 'primary'
  }
}

spark.on('login', () => {
  spark.getEventStream(false, 'mine', (event) => {
    console.log("Event: ", JSON.stringify(event))

    switch (event.name) {
      case 'spark/status':
        if (event.data === 'online') {
          rooms[event.coreid] = { coreOnline: true }
        } else {
          // end active events for this room
          // rooms[event.coreid]
        }
        break;

      case 'room_occupied':
        if (event.data === 'OCCUPIED') {
          const { googleCalendarId } = rooms[event.coreid]

          startEvent(googleCalendarId, { duration: DEFAULT_DURATION })
          .then((eventId) => {
            rooms[event.coreid] = {
              occupied: true,
              eventId
            }

            console.warn(eventId)
          })
        } else if (event.data === 'NOT OCCUPIED') {
          const { googleCalendarId, eventId } = rooms[event.coreid]

          endEvent(googleCalendarId, { eventId })
          .then(() => {
            rooms[event.coreid] = {
              occupied: false,
              eventId: null
            }
          })
        }
        break

      default:
        console.log(`Unknown event type: ${event}`)
        break
    }
  })
})

// spark.on('login', function() {
//   spark.getEventStream(false, 'mine', function(data) {
//     console.log("Event: ", JSON.stringify(data))
//   })
// })

spark.login({ accessToken: process.env.SPARK_TOKEN })


startEvent('primary', { duration: DEFAULT_DURATION }).then((eventId) => {
  console.warn('yo! resolved', eventId)
  console.warn('event', event)
  rooms[event.coreid] = {
    occupied: true,
    eventId
  }
}).catch((err) => {
  console.warn(err)
})
