import logoImg from "@assets/logo.png"
import EditorMenubar from "./EditorMenubar"
import DeviceDropdown from "./DeviceDropdown"


export default function Header() {

  return (
    <div className="w-full h-16 bg-neutral-900 flex items-center justify-between px-4">

        {/* Title */}
        <div className="flex items-center gap-4 -ml-4">

            <div className="w-16 square">
                <img src={logoImg} alt="ProjectDMX Logo" />
            </div>

            <div className="-ml-5">
                <h3 className="text-md ml-[6px] -mb-1 mt-1">Project Name</h3>

                <div>

                    <EditorMenubar/>

                </div>

            </div>

        </div>

        <DeviceDropdown/>

    </div>
  )
}