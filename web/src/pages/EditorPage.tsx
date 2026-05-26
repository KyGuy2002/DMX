export default function EditorPage() {

  return (
    <>
    
    {/* Effect Toolbar */}
      <section className="w-full h-16 bg-neutral-800 flex items-center justify-between px-4">



      </section>
    <section className="flex">

      
      {/* Prop Channels */}
      <div className="flex w-40 bg-neutral-900 border-r border-neutral-700 text-sm">

        {/* Prop Label Sideways */}
        <div className="w-7 shrink-0 flex items-center justify-center border-r border-b border-neutral-700">
          <div
            className="text-center whitespace-nowrap"
            style={{ writingMode: "vertical-rl", transform: "rotate(180deg)" }}
          >
            Arch Left
          </div>
        </div>

        {/* Channels List */}
        <div className="flex-1 flex flex-col">

          <div className="w-full border-b border-neutral-700 text-sm text-center py-1">
            Lantern
          </div>

          <div className="w-full border-b border-neutral-700 text-sm text-center py-1">
            Blinder
          </div>

          <div className="w-full border-b border-neutral-700 text-sm text-center py-1">
            Fog
          </div>

          <div className="w-full border-b border-neutral-700 text-sm text-center py-1">
            Window Glow
          </div>

        </div>

      </div>


      {/* Timeline Boxes */}
      <div className="grid grid-rows-4 w-full">

        <div className="border-b border-neutral-700">

          <div className="grid grid-cols-200">

            {[...Array(200)].map((_, i) => (
              <div key={i} className="w-[calc(100% / 200)] h-7 border-r border-neutral-700" />
            ))}

          </div>

        </div>



      </div>


      


      
      

    </section>
    </>
  )
}