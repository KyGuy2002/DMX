import logoImg from "@assets/logo.png"
import EditorMenubar from "./EditorMenubar"
import DeviceDropdown from "./DeviceDropdown"
import { Button } from "@/components/ui/button"
import { ArrowRightIcon } from "lucide-react"


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

                <div className="flex items-center gap-3">

                    <EditorMenubar/>

                </div>

            </div>

        </div>

        <div className="flex items-center gap-4">

            <Button variant="default" className="w-auto h-10 px-4">Deploy <ArrowRightIcon /></Button>

            <DeviceDropdown/>

        </div>

    </div>
  )
}